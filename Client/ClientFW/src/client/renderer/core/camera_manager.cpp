#include "stdafx.h"
#include "client/renderer/core/camera_manager.h"
#include "client/renderer/core/light_manager.h"
#include "client/renderer/core/render_resource_manager.h"
#include "client/renderer/frameresource/core/frame_resource_manager.h"
#include "client/renderer/frameresource/core/frame_resource.h"
#include "client/renderer/frameresource/camera_frame_resource.h"
#include "client/renderer/core/mesh_visualizer.h"

#include "client/object/component/util/render_camera_component.h"
#include "client/object/component/util/shadow_camera_component.h"
#include "client/object/component/util/shadow_cube_camera_component.h"
#include "client/object/component/util/shadow_cascade_camera_component.h"
#include "client/object/component/light/directional_light_component.h"

#include "client/asset/texture/texture.h"
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
		std::function<void(ID3D12Device*)>&& update_shader_function_for_shadow_camera,
		std::function<void(ID3D12Device*)>&& update_shader_function_for_shadow_cube_camera)
	{
		UpdateRenderCameras(device);
		UpdateShadowCameras(device);
		UpdateShadowCubeCameras(device);
		UpdateShadowCascadeCameras(device);

		UpdateCameraResource(device,
			std::move(update_shader_function_for_render_camera),
			std::move(update_shader_function_for_shadow_camera),
			std::move(update_shader_function_for_shadow_cube_camera));
	}

	void CameraManager::UpdateRenderCameras(ID3D12Device* device)
	{
		if (m_ready_main_camera != nullptr && m_ready_main_camera->GetRenderTexture() != nullptr &&
			m_main_camera != m_ready_main_camera)
		{
			m_main_camera = m_ready_main_camera;
			m_ready_main_camera = nullptr;
		}

		UpdateRenderCamerasForCascadeShadow(device);

		// 여기서 wait와 ready camera가 있는데 이와 같이 있는 이유는 
		// camera가 등록되면서 camera의 render texture를 초기화 할 필요가 있는데
		// 이때 GraphicsCommandList를 사용하기 때문에 이를 사용하기 전에 초기화 해줄 필요가 있어
		// ready camera를 등록하였고,
		// wait 카메라는 camera render texture가 초기화 되는 시점이 CamaraManger의 Update호출 후,
		// Draw함수 호출 전이기 때문에 Update한 카메라의 수와 Draw한 카메라의 수가 다르면 오류가 나기 때문에
		// 이렇게 wait camera 변수를 통해 기다려 주는 것이다.
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

	void CameraManager::UpdateRenderCamerasForCascadeShadow(ID3D12Device* device)
	{
		const auto& light_manager = LightManager::GetLightManager();
		const auto& ready_directional_lights = light_manager.GetReadyDirectionalLights();
		if (ready_directional_lights.empty() == false)
		{
			for (const auto& directional_light : ready_directional_lights)
			{
				for (const auto& camera : m_ready_render_cameras)
					directional_light->RegisterCascadeAndRenderCamera(camera, CreateSPtr<ShadowCascadeCameraComponent>());
				for (const auto& camera : m_wait_resource_render_cameras)
					directional_light->RegisterCascadeAndRenderCamera(camera, CreateSPtr<ShadowCascadeCameraComponent>());
				for (const auto& camera : m_render_cameras)
					directional_light->RegisterCascadeAndRenderCamera(camera, CreateSPtr<ShadowCascadeCameraComponent>());
			}
		}
		const auto& directional_lights = light_manager.GetDirectionalLights();
		if (directional_lights.empty() == false)
		{
			for (const auto& directional_light : ready_directional_lights)
			{
				for (const auto& camera : m_ready_render_cameras)
					directional_light->RegisterCascadeAndRenderCamera(camera, CreateSPtr<ShadowCascadeCameraComponent>());
				for (const auto& camera : m_wait_resource_render_cameras)
					directional_light->RegisterCascadeAndRenderCamera(camera, CreateSPtr<ShadowCascadeCameraComponent>());
			}
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

				camera->SetShadowTexture(CreateSPtr<Shadow2DTexture>(size));
				RenderResourceManager::GetRenderResourceManager().RegisterTexture(camera->GetShadowTexture());

				m_wait_resource_shadow_cameras.push_back(camera);
			}
			m_ready_shadow_cameras.clear();
		}
	}

	void CameraManager::UpdateShadowCubeCameras(ID3D12Device* device)
	{
		if (m_wait_resource_shadow_cube_cameras.empty() == false)
		{
			for (const auto& camera : m_wait_resource_shadow_cube_cameras)
				m_shadow_cube_cameras.push_back(camera);
			m_wait_resource_shadow_cube_cameras.clear();
		}

		if (m_ready_shadow_cube_cameras.empty() == false)
		{
			for (const auto& camera : m_ready_shadow_cube_cameras)
			{
				IVec2 size = IVec2(camera->GetViewport().width, camera->GetViewport().height);

				camera->SetShadowCubeTexture(CreateSPtr<ShadowCubeTexture>(size));
				RenderResourceManager::GetRenderResourceManager().RegisterTexture(camera->GetShadowCubeTexture());

				m_wait_resource_shadow_cube_cameras.push_back(camera);
			}
			m_ready_shadow_cube_cameras.clear();
		}
	}

	void CameraManager::UpdateShadowCascadeCameras(ID3D12Device* device)
	{
		if (m_wait_resource_shadow_cascade_cameras.empty() == false)
		{
			for (const auto& camera : m_wait_resource_shadow_cascade_cameras)
				m_shadow_cascade_cameras.push_back(camera);
			m_wait_resource_shadow_cascade_cameras.clear();
		}

		if (m_ready_shadow_cascade_cameras.empty() == false)
		{
			for (const auto& camera : m_ready_shadow_cascade_cameras)
			{
				camera->SetShadowArrayTexture(CreateSPtr<ShadowArrayTexture>(IVec2(2048, 2048), s_max_cascade_level));
				RenderResourceManager::GetRenderResourceManager().RegisterTexture(camera->GetShadowArrayTexture());

				m_wait_resource_shadow_cascade_cameras.push_back(camera);
			}
			m_ready_shadow_cascade_cameras.clear();
		}
	}

	void CameraManager::UpdateCameraResource(ID3D12Device* device,
		std::function<void(ID3D12Device*)>&& update_shader_function_for_render_camera,
		std::function<void(ID3D12Device*)>&& update_shader_function_for_shadow_camera,
		std::function<void(ID3D12Device*)>&& update_shader_function_for_shadow_cube_camera)
	{
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
					camera_data.view_projection_matrix = mat4::Transpose(camera->GetViewProjectionMatrix());
					camera_data.inverse_view_matrix = mat4::Transpose(camera->GetInverseViewMatrix());
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

					MeshVisualizer::UpdateVisibilityFromRenderCamera(camera);
					update_shader_function_for_render_camera(device);
				}
			}

			++index;
		}

		for (const auto& camera : m_shadow_cameras)
		{
			const auto& shadow_texture = camera->GetShadowTexture();

			if (camera->GetCameraState() == eCameraState::kActive && 
				shadow_texture->GetResource() != nullptr)
			{
				camera_data.view_projection_matrix = mat4::Transpose(camera->GetViewProjectionMatrix());
				camera_resource_data->CopyData(index, camera_data);

				MeshVisualizer::UpdateVisibilityFromShadowCamera(camera);
				update_shader_function_for_shadow_camera(device);
			}

			++index;
		}

		const auto& cube_camera_resource_data = camera_resource->GetCubeCameraData();

		count = static_cast<UINT>(m_shadow_cube_cameras.size());
		if (camera_resource->GetSizeOfCubeCamera() < count)
		{
			cube_camera_resource_data->CreateResource(device, count);
			camera_resource->SetSizeOfCubeCamera(count);
		}

		index = 0;

		for (const auto& camera : m_shadow_cube_cameras)
		{
			const auto& shadow_cube_texture = camera->GetShadowCubeTexture();

			if (camera->GetCameraState() == eCameraState::kActive &&
				shadow_cube_texture->GetResource() != nullptr)
			{
				RSCubeCameraData cube_camera_data;
				for (UINT i = 0; i < 6; ++i)
				{
					cube_camera_data.cube_view_projection[i] =
						mat4::Transpose(camera->GetCubeViewMatrixs()[i] * camera->GetProjectionMatrix());
				}
				cube_camera_resource_data->CopyData(index, cube_camera_data);

				MeshVisualizer::UpdateVisibliityFromShadowSphere(camera->GetWorldPosition(), camera->GetFarZ());
				update_shader_function_for_shadow_cube_camera(device);
			}

			++index;
		}

		index = 0;

		for (const auto& camera : m_shadow_cascade_cameras)
		{
			const auto& shadow_cascade_texture = camera->GetShadowArrayTexture();

			if (shadow_cascade_texture->GetResource() != nullptr)
			{
				//LOG_INFO("여기 어때?");
			}
		}
	}

	void CameraManager::UpdateMainCameraViewport(LONG width, LONG height)
	{
		//Resize (Texture 재생성)가 지원하지 않는 이상 무의미한 코드
		if (m_ready_main_camera != nullptr)
		{
			m_ready_main_camera->UpdateViewport(0, 0, width, height);
			m_ready_main_camera->SetNearZ(10.0f);
		}
		else if (m_main_camera != nullptr)
		{
			m_main_camera->UpdateViewport(0, 0, width, height);
			m_main_camera->SetNearZ(10.0f);
		}
	}

	void CameraManager::Draw(ID3D12GraphicsCommandList* command_list,
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

	void CameraManager::DrawShadow(ID3D12GraphicsCommandList* command_list,
		std::function<void(ID3D12GraphicsCommandList*)>&& shadow_function,
		std::function<void(ID3D12GraphicsCommandList*)>&& shadow_cube_function)
	{
		const auto& camera_resource = FrameResourceManager::GetManager().GetCurrentFrameResource()->GetCameraFrameResource();

		UINT index = static_cast<UINT>(m_render_cameras.size());
		D3D12_GPU_VIRTUAL_ADDRESS gpu_address;

		for (const auto& camera : m_shadow_cameras)
		{
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

				camera->SetPaused();
			}
			++index;
		}

		index = 0;

		for (const auto& camera : m_shadow_cube_cameras)
		{
			if (camera->GetCameraState() == eCameraState::kActive)
			{
				const auto& shadow_cube_texture = camera->GetShadowCubeTexture();

				if (shadow_cube_texture->GetResource() != nullptr)
				{
					gpu_address = camera_resource->GetCubeCameraData()->GetResource()->GetGPUVirtualAddress() +
						index * camera_resource->GetCubeCameraData()->GetByteSize();

					const auto& size = shadow_cube_texture->GetTextureSize();

					std::array<D3D12_VIEWPORT, 6> views;
					views.fill({ 0.f, 0.f, static_cast<float>(size.x), static_cast<float>(size.y), 0.0f, 1.0f });
					std::array<D3D12_RECT, 6> scissors;
					scissors.fill({ 0, 0, static_cast<LONG>(size.x), static_cast<LONG>(size.y) });

					command_list->RSSetViewports(6, views.data());
					command_list->RSSetScissorRects(6, scissors.data());

					command_list->SetGraphicsRootConstantBufferView(8, gpu_address);

					shadow_cube_texture->PreDraw(command_list);
					shadow_cube_function(command_list);
					shadow_cube_texture->PostDraw(command_list);

					camera->SetPaused();
				}
			}

			++index;
		}

		index = 0;

		for (const auto& camera : m_shadow_cascade_cameras)
		{
			const auto& shadow_cascade_texture = camera->GetShadowArrayTexture();

			if (shadow_cascade_texture->GetResource() != nullptr)
			{
				gpu_address = camera_resource->GetCubeCameraData()->GetResource()->GetGPUVirtualAddress() +
					index * camera_resource->GetCubeCameraData()->GetByteSize();

				const auto& size = shadow_cascade_texture->GetTextureSize();

				std::array<D3D12_VIEWPORT, s_max_cascade_level> views;
				views.fill({ 0.f, 0.f, static_cast<float>(size.x), static_cast<float>(size.y), 0.0f, 1.0f });
				std::array<D3D12_RECT, s_max_cascade_level> scissors;
				scissors.fill({ 0, 0, static_cast<LONG>(size.x), static_cast<LONG>(size.y) });

				command_list->RSSetViewports(s_max_cascade_level, views.data());
				command_list->RSSetScissorRects(s_max_cascade_level, scissors.data());

				//command_list->SetGraphicsRootConstantBufferView(8, gpu_address);

				shadow_cascade_texture->PreDraw(command_list);
				//shadow_cube_function(command_list);
				shadow_cascade_texture->PostDraw(command_list);

				//camera->SetPaused();
			}
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
		else if (camera_comp->GetCameraUsage() == eCameraUsage::kShadow)
			m_ready_shadow_cameras.push_back(std::static_pointer_cast<ShadowCameraComponent>(camera_comp));
		else if (camera_comp->GetCameraUsage() == eCameraUsage::kShadowCube)
			m_ready_shadow_cube_cameras.push_back(std::static_pointer_cast<ShadowCubeCameraComponent>(camera_comp));
		else
			m_ready_shadow_cascade_cameras.push_back(std::static_pointer_cast<ShadowCascadeCameraComponent>(camera_comp));
		return true;
	}

	void CameraManager::UnregisterCameraComponent(const SPtr<CameraComponent>& camera_comp)
	{
		switch (camera_comp->GetCameraUsage())
		{
		case eCameraUsage::kBasic:
		{
			if (camera_comp == m_main_camera)
				m_main_camera = nullptr;

			UnregisterCameraComponent(m_ready_render_cameras, camera_comp);
			UnregisterCameraComponent(m_wait_resource_render_cameras, camera_comp);
			UnregisterCameraComponent(m_render_cameras, camera_comp);
			UnregisterRenderCameraToDirectionalLights(std::static_pointer_cast<RenderCameraComponent>(camera_comp));
			break;
		}
		case eCameraUsage::kShadow:
		{
			UnregisterCameraComponent(m_ready_shadow_cameras, camera_comp);
			UnregisterCameraComponent(m_wait_resource_shadow_cameras, camera_comp);
			UnregisterCameraComponent(m_shadow_cameras, camera_comp);
			break;
		}
		case eCameraUsage::kShadowCube:
		{
			UnregisterCameraComponent(m_ready_shadow_cube_cameras, camera_comp);
			UnregisterCameraComponent(m_wait_resource_shadow_cube_cameras, camera_comp);
			UnregisterCameraComponent(m_shadow_cube_cameras, camera_comp);
			break;
		}
		case eCameraUsage::kShadowCascade:
		{
			UnregisterCameraComponent(m_ready_shadow_cascade_cameras, camera_comp);
			UnregisterCameraComponent(m_wait_resource_shadow_cascade_cameras, camera_comp);
			UnregisterCameraComponent(m_shadow_cascade_cameras, camera_comp);
			break;
		}
		default:
			break;
		}
	}

	void CameraManager::UnregisterRenderCameraToDirectionalLights(const SPtr<RenderCameraComponent>& camera_comp)
	{
		const auto& light_manager = LightManager::GetLightManager();
		const auto& ready_directional_lights = light_manager.GetReadyDirectionalLights();

		for (const auto& light : ready_directional_lights)
			light->UnregisterRenderCamera(camera_comp);

		const auto& directional_lights = light_manager.GetDirectionalLights();

		for (const auto& light : directional_lights)
			light->UnregisterRenderCamera(camera_comp);
	}

}
