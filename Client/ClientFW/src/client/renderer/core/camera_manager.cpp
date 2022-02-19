#include "stdafx.h"
#include "client/renderer/core/camera_manager.h"
#include "client/renderer/core/render_resource_manager.h"
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
		m_camera_data = CreateUPtr<UploadBuffer<RSCameraData>>(true);
	}

	CameraManager::~CameraManager()
	{
	}

	void CameraManager::Shutdown()
	{
		m_camera_data->Shutdown();
	}

	void CameraManager::Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
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

				camera->SetRenderTexture(CreateSPtr<RenderTexture>());

				//임시로 rtv_format은 이렇게 저장하자.
				//어떻게 OpaqueRenderLevel(일단은 Opaque만 GBuffer를 사용하기 때문에)과 맞출지 생각하자.
				camera->GetRenderTexture()->Initialize(device, command_list,
					{}, size);
				RenderResourceManager::GetRenderResourceManager().RegisterTexture(camera->GetRenderTexture());

				m_cameras[eCameraUsage::kBasic].push_back(camera);
			}
			m_ready_cameras[eCameraUsage::kBasic].clear();

			CreateCameraResource(device);
		}
		UpdateCameraResource();
	}

	void CameraManager::UpdateMainCameraViewport(LONG left, LONG top, LONG width, LONG height)
	{
		if (m_ready_main_camera != nullptr)
			m_ready_main_camera->UpdateViewport(left, top, width, height);
		else if(m_main_camera != nullptr)
			m_main_camera->UpdateViewport(left, top, width, height);
	}

	void CameraManager::Draw(ID3D12GraphicsCommandList* command_list, std::function<void(ID3D12GraphicsCommandList*)>&& function)
	{
		UINT index = 0;

		for (const auto& camera : m_cameras[eCameraUsage::kBasic])
		{
			if (camera->GetCameraState() == eCameraState::kActive)
			{
				command_list->SetGraphicsRootConstantBufferView(2, m_camera_data->GetResource()->GetGPUVirtualAddress() +
					index * m_camera_data->GetByteSize());
				MeshVisualizer::UpdateVisibilityFromCamera(camera);

				camera->GetRenderTexture()->PreDraw(command_list);
				function(command_list);
				camera->GetRenderTexture()->PostDraw(command_list);
			}
			++index;
		}
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
		}
	}

	void CameraManager::CreateCameraResource(ID3D12Device* device)
	{
		m_camera_data->CreateResource(device, static_cast<UINT>(m_cameras.size()));
	}

	void CameraManager::UpdateCameraResource()
	{
		RSCameraData camera_data;
		UINT index = 0;
		for (const auto& camera : m_cameras[eCameraUsage::kBasic])
		{
			camera_data.view_matrix = mat4::Transpose(camera->GetViewMatrix());
			camera_data.projection_matrix = mat4::Transpose(camera->GetProjectionMatrix());
			camera_data.camera_position = camera->GetWorldPosition();
			m_camera_data->CopyData(index, camera_data);
			++index;
		}
	}

	void CameraManager::UpdateCameraResourceBeforeDraw()
	{
	}	
}
