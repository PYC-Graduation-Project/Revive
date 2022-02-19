#include "stdafx.h"
#include "client/renderer/core/camera_manager.h"
#include "client/object/component/util/camera_component.h"
#include "client/asset/texture/texture.h"
#include "client/renderer/core/render_resource_manager.h"

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

				//AssetStore::LoadRenderTexture("main camera");

				camera->SetRenderTexture(CreateSPtr<RenderTexture>());

				//임시로 rtv_format은 이렇게 저장하자.
				//어떻게 OpaqueRenderLevel(일단은 Opaque만 GBuffer를 사용하기 때문에)과 맞출지 생각하자.
				camera->GetRenderTexture()->Initialize(device, command_list,
					{}, size);
				RenderResourceManager::GetRenderResourceManager().RegisterTexture(camera->GetRenderTexture());

				m_cameras[eCameraUsage::kBasic].push_back(camera);
			}
			m_ready_cameras[eCameraUsage::kBasic].clear();
		}
	}

	void CameraManager::UpdateMainCameraViewport(LONG left, LONG top, LONG width, LONG height)
	{
		if (m_ready_main_camera != nullptr)
			m_ready_main_camera->UpdateViewport(left, top, width, height);
		else if(m_main_camera != nullptr)
			m_main_camera->UpdateViewport(left, top, width, height);
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
	
}
