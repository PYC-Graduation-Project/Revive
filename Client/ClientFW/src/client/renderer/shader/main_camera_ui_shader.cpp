#include "stdafx.h"
#include "client/renderer/core/render.h"
#include "client/renderer/core/camera_manager.h"
#include "client/renderer/shader/main_camera_ui_shader.h"
#include "client/renderer/renderlevel/core/render_level.h"
#include "client/object/component/util/camera_component.h"
#include "client/asset/texture/texture.h"
#include "client/util/upload_buffer.h"

namespace client_fw
{
	MainCameraUIShader::MainCameraUIShader(const std::string& name)
		: GraphicsShader(name)
	{
		m_texture_index_data = CreateUPtr<UploadBuffer<RSUITextureIndexData>>(true);
	}

	void MainCameraUIShader::Initialize(ID3D12Device* device)
	{
		m_texture_index_data->CreateResource(device, 1);
	}

	void MainCameraUIShader::Shutdown()
	{
	}

	void MainCameraUIShader::Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type)
	{
		const auto& main_camera = CameraManager::GetCameraManager().GetMainCamera();
		if (main_camera != nullptr)
		{
			m_texture_index_data->CopyData(0, RSUITextureIndexData{
			CameraManager::GetCameraManager().GetMainCamera()->GetRenderTexture()->GetResourceIndex()
				});
		}
	}

	void MainCameraUIShader::Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type) const
	{
		switch (level_type)
		{
		case eRenderLevelType::kUI:
			command_list->SetPipelineState(m_pipeline_states.at(level_type)[0].Get());
			command_list->SetGraphicsRootConstantBufferView(5, m_texture_index_data->GetResource()->GetGPUVirtualAddress());
			command_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			command_list->DrawInstanced(6, 1, 0, 0);
			break;
		default:
			break;
		}
	}
	D3D12_SHADER_BYTECODE MainCameraUIShader::CreateVertexShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		return CompileShader(L"../ClientFW/src/client/renderer/hlsl/UI.hlsl", "VSRenderMainUI", "vs_5_1", shader_blob);
	}

	D3D12_SHADER_BYTECODE MainCameraUIShader::CreatePixelShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		return CompileShader(L"../ClientFW/src/client/renderer/hlsl/UI.hlsl", "PSRenderMainUI", "ps_5_1", shader_blob);
	}

	D3D12_DEPTH_STENCIL_DESC MainCameraUIShader::CreateDepthStencilState(eRenderLevelType level_type, int pso_index) const
	{
		D3D12_DEPTH_STENCIL_DESC desc = GraphicsShader::CreateDepthStencilState(level_type, pso_index);
		desc.DepthEnable = FALSE;
		return desc;
	}

	bool MainCameraUIShader::CreatePipelineStates(ID3D12Device* device, const SPtr<GraphicsRenderLevel>& render_level)
	{
		bool result = true;
		
		switch (render_level->GetRenderLevelType())
		{
		case eRenderLevelType::kUI:
			result &= CreatePipelineState(device, render_level, 1);
			break;
		default:
			LOG_ERROR("Could not support {0} from {1}",
				Render::ConvertRenderLevelType(render_level->GetRenderLevelType()), m_name);
			return false;
		}

		return result;
	}
}
