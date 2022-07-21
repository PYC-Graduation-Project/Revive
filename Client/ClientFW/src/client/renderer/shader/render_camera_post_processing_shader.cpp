#include "stdafx.h"
#include "client/renderer/core/render.h"
#include "client/renderer/shader/render_camera_post_processing_shader.h"
#include "client/renderer/renderlevel/core/render_level.h"
#include "client/object/component/util/render_camera_component.h"
#include "client/asset/texture/texture.h"
#include "client/util/upload_buffer.h"

namespace client_fw
{
	RenderCameraPostProcessingShader::RenderCameraPostProcessingShader(const std::string& name)
		: ComputeShader(name)
	{
	}

	void RenderCameraPostProcessingShader::Initialize(ID3D12Device* device)
	{
	}

	void RenderCameraPostProcessingShader::Shutdown()
	{
	}

	void RenderCameraPostProcessingShader::Update(ID3D12Device* device, eRenderLevelType level_type)
	{
	}

	void RenderCameraPostProcessingShader::UpdateFrameResource(ID3D12Device* device, eRenderLevelType level_type)
	{
	}

	void RenderCameraPostProcessingShader::Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type) const
	{
		for (const auto& camera : m_render_cameras)
		{
			if (camera->GetCameraState() == eCameraState::kActive)
			{
				DrawSobelEdge(command_list, level_type, camera);
			}
		}
	}

	void RenderCameraPostProcessingShader::DrawSobelEdge(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type, 
		const SPtr<RenderCameraComponent>& camera) const
	{
		const auto& render_texture = camera->GetRenderTexture();
		const auto& blur_texture1 = camera->GetRWTexture("pp texture 1");

		if (blur_texture1->GetResource() != nullptr)
		{
			const auto& post_procssing_info = camera->GetPostProcessingInfo();

			if (post_procssing_info->use_sobel_edge)
			{
				command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(blur_texture1->GetResource(),
					D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_UNORDERED_ACCESS));

				command_list->SetPipelineState(m_pipeline_states.at(level_type)[0].Get());
				command_list->SetComputeRootDescriptorTable(0, render_texture->GetGPUHandle());
				command_list->SetComputeRootDescriptorTable(1, blur_texture1->GetGPUHandle());

				UINT num_group_x = static_cast<UINT>(ceilf(static_cast<float>(blur_texture1->GetTextureSize().x) / 16.0f));
				UINT num_group_y = static_cast<UINT>(ceilf(static_cast<float>(blur_texture1->GetTextureSize().y) / 16.0f));

				command_list->Dispatch(num_group_x, num_group_y, 1);

				command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(render_texture->GetResource(),
					D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_COPY_DEST));
				command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(blur_texture1->GetResource(),
					D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_SOURCE));

				command_list->CopyResource(render_texture->GetResource(), blur_texture1->GetResource());

				command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(render_texture->GetResource(),
					D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ));
				command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(blur_texture1->GetResource(),
					D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_COMMON));
			}
		}
	}

	D3D12_SHADER_BYTECODE RenderCameraPostProcessingShader::CreateComputeShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		switch (pso_index)
		{
		case 0:
			return CompileShader(L"../ClientFW/src/client/renderer/hlsl/RenderCameraPostProcessing.hlsl", "CSSobelEdge", "cs_5_1", shader_blob);
		default:
			return D3D12_SHADER_BYTECODE();
		}
	}

	bool RenderCameraPostProcessingShader::CreatePipelineStates(ID3D12Device* device, const SPtr<ComputeRenderLevel>& render_level)
	{
		bool result = true;

		switch (render_level->GetRenderLevelType())
		{
		case eRenderLevelType::kPostProcess:
			result &= CreatePipelineState(device, render_level, 1);
			break;
		default:
			LOG_ERROR("Could not support {0} from {1}",
				Render::ConvertRenderLevelType(render_level->GetRenderLevelType()), m_name);
			return false;
		}

		return result;
	}

	bool RenderCameraPostProcessingShader::RegisterCameraComponent(const SPtr<CameraComponent>& camera_comp)
	{
		const auto& render_camera = std::dynamic_pointer_cast<RenderCameraComponent>(camera_comp);
		if (render_camera != nullptr && render_camera->GetPostProcessingInfo()->use_post_processing)
		{
			m_render_cameras.push_back(render_camera);
			render_camera->SetRWTexture("pp texture 1", CreateSPtr<RWTexture>());
			render_camera->SetRWTexture("pp texture 2", CreateSPtr<RWTexture>());
			return true;
		}
		return false;
	}

	void RenderCameraPostProcessingShader::UnregisterCameraComponent(const SPtr<CameraComponent>& camera_comp)
	{
		const auto& render_camera = std::dynamic_pointer_cast<RenderCameraComponent>(camera_comp);
		if (render_camera != nullptr)
		{
			auto iter = std::find(m_render_cameras.begin(), m_render_cameras.end(), render_camera);
			if (iter != m_render_cameras.end())
			{
				std::iter_swap(iter, m_render_cameras.end() - 1);
				m_render_cameras.pop_back();
			}
		}
	}
}