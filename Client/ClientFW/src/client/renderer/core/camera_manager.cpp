#include "stdafx.h"
#include "client/renderer/core/camera_manager.h"
#include "client/renderer/core/light_manager.h"
#include "client/renderer/core/render_resource_manager.h"
#include "client/renderer/frameresource/core/frame_resource_manager.h"
#include "client/renderer/frameresource/core/frame_resource.h"
#include "client/renderer/frameresource/camera_frame_resource.h"
#include "client/object/component/util/render_camera_component.h"
#include "client/object/component/util/shadow_camera_component.h"
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

	void CameraManager::Update(ID3D12Device* device,
		std::function<void(ID3D12Device*)>&& update_shader_function_for_render_camera,
		std::function<void(ID3D12Device*)>&& update_shader_function_for_shadow_camera)
	{
		UpdateRenderCameras(device);
		UpdateShadowCameras(device);

		UpdateCameraResource(device,
			std::move(update_shader_function_for_render_camera),
			std::move(update_shader_function_for_shadow_camera));
	}

	void CameraManager::UpdateRenderCameras(ID3D12Device* device)
	{
		if (m_ready_main_camera != nullptr && m_ready_main_camera->GetRenderTexture() != nullptr &&
			m_main_camera != m_ready_main_camera)
		{
			m_main_camera = m_ready_main_camera;
			m_ready_main_camera = nullptr;
		}

		// ���⼭ wait�� ready camera�� �ִµ� �̿� ���� �ִ� ������ 
		// camera�� ��ϵǸ鼭 camera�� render texture�� �ʱ�ȭ �� �ʿ䰡 �ִµ�
		// �̶� GraphicsCommandList�� ����ϱ� ������ �̸� ����ϱ� ���� �ʱ�ȭ ���� �ʿ䰡 �־�
		// ready camera�� ����Ͽ���,
		// wait ī�޶�� camera render texture�� �ʱ�ȭ �Ǵ� ������ CamaraManger�� Updateȣ�� ��,
		// Draw�Լ� ȣ�� ���̱� ������ Update�� ī�޶��� ���� Draw�� ī�޶��� ���� �ٸ��� ������ ���� ������
		// �̷��� wait camera ������ ���� ��ٷ� �ִ� ���̴�.
		if (m_wait_resource_render_cameras.empty() == false)
		{
			for (const auto& camera : m_wait_resource_render_cameras)
				m_render_cameras.push_back(camera);
			m_wait_resource_render_cameras.clear();
		}

		if (m_ready_render_cameras.empty() == false)
		{
			for (const auto& camera : m_ready_render_cameras)
			{
				IVec2 size = IVec2(camera->GetViewport().width, camera->GetViewport().height);

				camera->SetRenderTexture(CreateSPtr<RenderTexture>(size));
				RenderResourceManager::GetRenderResourceManager().RegisterTexture(camera->GetRenderTexture());

				m_wait_resource_render_cameras.push_back(camera);
			}
			m_ready_render_cameras.clear();
		}	
	}

	void CameraManager::UpdateShadowCameras(ID3D12Device* device)
	{
		if (m_wait_resource_shadow_cameras.empty() == false)
		{
			for (const auto& camera : m_wait_resource_shadow_cameras)
				m_shadow_cameras.push_back(camera);
			m_wait_resource_shadow_cameras.clear();
		}

		if (m_ready_shadow_cameras.empty() == false)
		{
			for (const auto& camera : m_ready_shadow_cameras)
			{
				IVec2 size = IVec2(camera->GetViewport().width, camera->GetViewport().height);

				camera->SetShadowTexture(CreateSPtr<ShadowTexture>(size));
				RenderResourceManager::GetRenderResourceManager().RegisterTexture(camera->GetShadowTexture());

				m_wait_resource_shadow_cameras.push_back(camera);
			}
			m_ready_shadow_cameras.clear();
		}
	}

	void CameraManager::UpdateCameraResource(ID3D12Device* device,
		std::function<void(ID3D12Device*)>&& update_shader_function_for_render_camera,
		std::function<void(ID3D12Device*)>&& update_shader_function_for_shadow_camera)
	{
		if (m_render_cameras.empty() && m_shadow_cameras.empty())
			return;

		const auto& camera_resource = FrameResourceManager::GetManager().GetCurrentFrameResource()->GetCameraFrameResource();
		const auto& camera_resource_data = camera_resource->GetCameraData();

		UINT count = static_cast<UINT>(m_render_cameras.size() + 1 + m_shadow_cameras.size());
		if (camera_resource->GetSizeOfCamera() < count)
		{
			camera_resource_data->CreateResource(device, count);
			camera_resource->SetSizeOfCamera(count);
		}

		const auto& light_manager = LightManager::GetLightManager();
		UINT num_of_directional_light = static_cast<UINT>(light_manager.GetDirectionalLights().size());

		RSCameraData camera_data;
		UINT index = 0;

		for (const auto& camera : m_render_cameras)
		{
			if (camera->GetCameraState() == eCameraState::kActive)
			{
				const auto& render_texture = camera->GetRenderTexture();

				if (render_texture->GetResource() != nullptr)
				{
					camera_data.view_matrix = mat4::Transpose(camera->GetViewMatrix());
					camera_data.projection_matrix = mat4::Transpose(camera->GetProjectionMatrix());
					camera_data.view_projection_matrix = camera_data.projection_matrix * camera_data.view_matrix;
					camera_data.inverse_view_matrix = mat4::Inverse(camera_data.view_matrix);
					camera_data.perspective_values = Vec4(1.0f / camera_data.projection_matrix._11,
						1.0f / camera_data.projection_matrix._22,
						camera_data.projection_matrix._34,
						-camera_data.projection_matrix._33);
					camera_data.camera_position = camera->GetCameraPosition();
					camera_data.final_texture_index = render_texture->GetResourceIndex();
					camera_data.gbuffer_texture_indices = XMUINT4(
						render_texture->GetGBufferResourceIndex(0), render_texture->GetGBufferResourceIndex(1),
						render_texture->GetGBufferResourceIndex(2), render_texture->GetDSVResourceIndex());
					camera_data.num_of_directional_light = num_of_directional_light;
					camera_resource_data->CopyData(index, camera_data);

					if (camera == m_main_camera)
					{
						camera_data.projection_matrix = mat4::Transpose(camera->GetOrthoMatrix());
						camera_resource_data->CopyData(static_cast<UINT>(m_render_cameras.size() + m_shadow_cameras.size()), camera_data);
					}

					MeshVisualizer::UpdateVisibilityFromCamera(camera);
					update_shader_function_for_render_camera(device);
				}
			}

			++index;
		}

		for (const auto& camera : m_shadow_cameras)
		{
			const auto& shadow_texture = camera->GetShadowTexture();

			if (camera->GetCameraState() == eCameraState::kActive)
			{
				if (shadow_texture->GetResource() != nullptr)
				{
					camera_data.view_matrix = mat4::Transpose(camera->GetViewMatrix());
					camera_data.projection_matrix = mat4::Transpose(camera->GetProjectionMatrix());
					camera_data.view_projection_matrix = camera_data.projection_matrix * camera_data.view_matrix;
					camera_data.inverse_view_matrix = mat4::Inverse(camera_data.view_matrix);
					camera_data.perspective_values = Vec4(1.0f / camera_data.projection_matrix._11,
						1.0f / camera_data.projection_matrix._22,
						camera_data.projection_matrix._34,
						-camera_data.projection_matrix._33);
					camera_data.camera_position = camera->GetCameraPosition();
					camera_data.final_texture_index = shadow_texture->GetResourceIndex();
					camera_resource_data->CopyData(index, camera_data);

					MeshVisualizer::UpdateVisibilityFromCamera(camera);
					update_shader_function_for_shadow_camera(device);
				}
			}

			++index;
		}
	}

	void CameraManager::UpdateMainCameraViewport(LONG width, LONG height)
	{
		//Resize (Texture �����)�� �������� �ʴ� �̻� ���ǹ��� �ڵ�
		if (m_ready_main_camera != nullptr)
			m_ready_main_camera->UpdateViewport(0, 0, width, height);
		else if(m_main_camera != nullptr)
			m_main_camera->UpdateViewport(0, 0, width, height);
	}

	void CameraManager::Draw(ID3D12GraphicsCommandList* command_list,
		std::function<void(ID3D12GraphicsCommandList*)>&& shadow_function,
		std::function<void(ID3D12GraphicsCommandList*)>&& before_deferred_function,
		std::function<void(ID3D12GraphicsCommandList*)>&& deferred_function, 
		std::function<void(ID3D12GraphicsCommandList*)>&& after_deferred_function)
	{
		const auto& camera_resource = FrameResourceManager::GetManager().GetCurrentFrameResource()->GetCameraFrameResource();

		UINT index = 0;

		for (const auto& camera : m_render_cameras)
		{
			D3D12_GPU_VIRTUAL_ADDRESS gpu_address;

			if (camera->GetCameraState() == eCameraState::kActive)
			{
				const auto& render_texture = camera->GetRenderTexture();

				if (render_texture->GetResource() != nullptr)
				{
					gpu_address = camera_resource->GetCameraData()->GetResource()->GetGPUVirtualAddress() +
						index * camera_resource->GetCameraData()->GetByteSize();

					//�ϴ� �ӽ�, ���Ŀ� resize�۾��� �ϰ� �Ǹ� ����
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

		for (const auto& camera : m_shadow_cameras)
		{
			D3D12_GPU_VIRTUAL_ADDRESS gpu_address;

			if (camera->GetCameraState() == eCameraState::kActive)
			{
				const auto& shadow_texture = camera->GetShadowTexture();

				if (shadow_texture->GetResource() != nullptr)
				{
					gpu_address = camera_resource->GetCameraData()->GetResource()->GetGPUVirtualAddress() +
						index * camera_resource->GetCameraData()->GetByteSize();

					const auto& cv = shadow_texture->GetTextureSize();
					D3D12_VIEWPORT view = { 0.f, 0.f, static_cast<float>(cv.x), static_cast<float>(cv.y), 0.0f, 1.0f };
					D3D12_RECT scissor = { 0, 0, static_cast<LONG>(cv.x), static_cast<LONG>(cv.y) };
					command_list->RSSetViewports(1, &view);
					command_list->RSSetScissorRects(1, &scissor);

					command_list->SetGraphicsRootConstantBufferView(2, gpu_address);

					shadow_texture->PreDraw(command_list);
					shadow_function(command_list);
					shadow_texture->PostDraw(command_list);
				}
			}
			++index;
		}
	}

	void CameraManager::DrawMainCameraForUI(ID3D12GraphicsCommandList* command_list)
	{
		const auto& camera_resource = FrameResourceManager::GetManager().GetCurrentFrameResource()->GetCameraFrameResource();

		command_list->SetGraphicsRootConstantBufferView(2, camera_resource->GetCameraData()->GetResource()->GetGPUVirtualAddress() +
			(m_render_cameras.size() + m_shadow_cameras.size()) * camera_resource->GetCameraData()->GetByteSize());
	}

	bool CameraManager::RegisterCameraComponent(const SPtr<CameraComponent>& camera_comp)
	{
		if (camera_comp->GetCameraUsage() == eCameraUsage::kBasic)
			m_ready_render_cameras.push_back(std::static_pointer_cast<RenderCameraComponent>(camera_comp));
		else
			m_ready_shadow_cameras.push_back(std::static_pointer_cast<ShadowCameraComponent>(camera_comp));
		return true;
	}

	void CameraManager::UnregisterCameraComponent(const SPtr<CameraComponent>& camera_comp)
	{
		if (camera_comp->GetCameraUsage() == eCameraUsage::kBasic)
		{
			if (camera_comp == m_main_camera)
				m_main_camera = nullptr;

			UnregisterCameraComponent(m_ready_render_cameras, camera_comp);
			UnregisterCameraComponent(m_wait_resource_render_cameras, camera_comp);
			UnregisterCameraComponent(m_render_cameras, camera_comp);
		}
		else
		{
			UnregisterCameraComponent(m_ready_shadow_cameras, camera_comp);
			UnregisterCameraComponent(m_wait_resource_shadow_cameras, camera_comp);
			UnregisterCameraComponent(m_shadow_cameras, camera_comp);
		}
	}
}
