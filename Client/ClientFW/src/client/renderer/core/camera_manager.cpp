#include "stdafx.h"
#include "client/renderer/core/camera_manager.h"
#include "client/renderer/core/light_manager.h"
#include "client/renderer/core/render_resource_manager.h"
#include "client/renderer/frameresource/core/frame_resource_manager.h"
#include "client/renderer/frameresource/core/frame_resource.h"
#include "client/renderer/frameresource/camera_frame_resource.h"
#include "client/object/component/util/camera_component.h"
#include "client/asset/texture/texture.h"
#include "client/renderer/core/mesh_visualizer.h"
#include "client/util/upload_buffer.h"

namespace client_fw
{
	CameraManager* CameraManager::s_camera_manager = nullptr;

	CameraManager::CameraManager()
	{
		s_camera_manager = this;
	}

	CameraManager::~CameraManager()
	{
	}

	void CameraManager::Shutdown()
	{
	}

	void CameraManager::Update(ID3D12Device* device, std::function<void(ID3D12Device*)>&& update_shader_function)
	{
		//더 좋은 방법 없을까?
		if (m_ready_main_camera != nullptr && m_ready_main_camera->GetRenderTexture() != nullptr && 
			m_main_camera != m_ready_main_camera)
		{
			m_main_camera = m_ready_main_camera;
			m_ready_main_camera = nullptr;
		}

		if (m_ready_cameras[eCameraUsage::kBasic].empty() == false)
		{
			for (const auto& camera : m_ready_cameras[eCameraUsage::kBasic])
			{
				IVec2 size = IVec2(camera->GetViewport().width, camera->GetViewport().height);

				camera->SetRenderTexture(CreateSPtr<RenderTexture>(size));
				RenderResourceManager::GetRenderResourceManager().RegisterTexture(camera->GetRenderTexture());

				m_cameras[eCameraUsage::kBasic].push_back(camera);
			}
			m_ready_cameras[eCameraUsage::kBasic].clear();
		}

		UpdateCameraResource(device, std::move(update_shader_function));
	}

	void CameraManager::UpdateMainCameraViewport(LONG width, LONG height)
	{
		//Resize (Texture 재생성)가 지원하지 않는 이상 무의미한 코드
		if (m_ready_main_camera != nullptr)
			m_ready_main_camera->UpdateViewport(0, 0, width, height);
		else if(m_main_camera != nullptr)
			m_main_camera->UpdateViewport(0, 0, width, height);
	}

	void CameraManager::Draw(ID3D12GraphicsCommandList* command_list,
		std::function<void(ID3D12GraphicsCommandList*)>&& before_deferred_function,
		std::function<void(ID3D12GraphicsCommandList*)>&& deferred_function, 
		std::function<void(ID3D12GraphicsCommandList*)>&& after_deferred_function)
	{
		const auto& camera_resource = FrameResourceManager::GetManager().GetCurrentFrameResource()->GetCameraFrameResource();

		UINT index = 0;

		for (const auto& camera : m_cameras[eCameraUsage::kBasic])
		{
			D3D12_GPU_VIRTUAL_ADDRESS gpu_address;

			if (camera->GetCameraState() == eCameraState::kActive)
			{
				const auto& render_texture = camera->GetRenderTexture();

				if (render_texture->GetResource() != nullptr)
				{
					gpu_address = camera_resource->GetCameraData()->GetResource()->GetGPUVirtualAddress() +
						index * camera_resource->GetCameraData()->GetByteSize();

					//일단 임시, 추후에 resize작업을 하게 되면 설정
					const auto& cv = camera->GetRenderTexture()->GetTextureSize();
					D3D12_VIEWPORT view = { 0.f, 0.f, static_cast<float>(cv.x), static_cast<float>(cv.y), 0.0f, 1.0f };
					D3D12_RECT scissor = { 0, 0, static_cast<LONG>(cv.x), static_cast<LONG>(cv.y) };
					command_list->RSSetViewports(1, &view);
					command_list->RSSetScissorRects(1, &scissor);

					command_list->SetGraphicsRootConstantBufferView(2, gpu_address);

					camera->GetRenderTexture()->GBufferPreDraw(command_list);
					before_deferred_function(command_list);
					camera->GetRenderTexture()->GBufferPostDraw(command_list);

					camera->GetRenderTexture()->PreDraw(command_list);
					deferred_function(command_list);
					camera->GetRenderTexture()->PostDraw(command_list);
				}
			}
			++index;
		}
	}

	void CameraManager::DrawMainCameraForUI(ID3D12GraphicsCommandList* command_list)
	{
		const auto& camera_resource = FrameResourceManager::GetManager().GetCurrentFrameResource()->GetCameraFrameResource();

		command_list->SetGraphicsRootConstantBufferView(2, camera_resource->GetCameraData()->GetResource()->GetGPUVirtualAddress() +
			m_cameras[eCameraUsage::kBasic].size() * camera_resource->GetCameraData()->GetByteSize());
	}

	bool CameraManager::RegisterCameraComponent(const SPtr<CameraComponent>& camera_comp)
	{
		m_ready_cameras[camera_comp->GetCameraUsage()].push_back(camera_comp);
		return true;
	}

	void CameraManager::UnregisterCameraComponent(const SPtr<CameraComponent>& camera_comp)
	{
		auto& cameras = m_cameras[camera_comp->GetCameraUsage()];

		auto iter = std::find(cameras.begin(), cameras.end(), camera_comp);
		if (iter != cameras.end())
		{
			std::iter_swap(iter, cameras.end() - 1);
			cameras.pop_back();
			if (camera_comp == m_main_camera)
				m_main_camera = nullptr;
		}

		auto& ready_cameras = m_ready_cameras[camera_comp->GetCameraUsage()];

		iter = std::find(ready_cameras.begin(), ready_cameras.end(), camera_comp);
		if (iter != ready_cameras.end())
		{
			std::iter_swap(iter, ready_cameras.end() - 1);
			ready_cameras.pop_back();
			if (camera_comp == m_main_camera)
				m_main_camera = nullptr;
		}
	}

	void CameraManager::UpdateCameraResource(ID3D12Device* device, std::function<void(ID3D12Device*)>&& update_shader_function)
	{
		if (m_cameras[eCameraUsage::kBasic].empty())
			return;

		const auto& camera_resource = FrameResourceManager::GetManager().GetCurrentFrameResource()->GetCameraFrameResource();
		const auto& camera_resource_data = camera_resource->GetCameraData();

		if (camera_resource->GetNumOfCamera() < m_cameras[eCameraUsage::kBasic].size() + 1)
		{
			UINT count = static_cast<UINT>(m_cameras[eCameraUsage::kBasic].size()) + 1;
			camera_resource_data->CreateResource(device, count);
			camera_resource->SetNumOfCamera(count);
		}

		const auto& light_manager = LightManager::GetLightManager();
		UINT num_of_directional_light = static_cast<UINT>(light_manager.GetDirectionalLights().size());

		RSCameraData camera_data;
		UINT index = 0;

		for (const auto& camera : m_cameras[eCameraUsage::kBasic])
		{
			if (camera->GetCameraState() == eCameraState::kActive)
			{
				const auto& render_texture = camera->GetRenderTexture();

				if (render_texture->GetResource() != nullptr)
				{
					camera_data.view_matrix = mat4::Transpose(camera->GetViewMatrix());
					camera_data.projection_matrix = mat4::Transpose(camera->GetProjectionMatrix());
					camera_data.view_projection_matrix = camera_data.projection_matrix * camera_data.view_matrix;
					camera_data.camera_position = camera->GetWorldPosition();
					camera_data.final_texture_index = render_texture->GetResourceIndex();
					camera_data.gbuffer_texture_indices = XMUINT4(
						render_texture->GetGBufferResourceIndex(0), render_texture->GetGBufferResourceIndex(1),
						0, render_texture->GetDSVResourceIndex());
					camera_data.num_of_directional_light = num_of_directional_light;
					camera_resource_data->CopyData(index, camera_data);

					if (camera == m_main_camera)
					{
						camera_data.projection_matrix = mat4::Transpose(camera->GetOrthoMatrix());
						camera_resource_data->CopyData(static_cast<UINT>(m_cameras[eCameraUsage::kBasic].size()), camera_data);
					}

					MeshVisualizer::UpdateVisibilityFromCamera(camera);
					update_shader_function(device);
				}
			}
			
			++index;
		}
	}
}
