#include "stdafx.h"
#include "client/core/window.h"
#include "client/renderer/core/render.h"
#include "client/renderer/core/render_system.h"
#include "client/renderer/rootsignature/graphics_super_root_signature.h"
#include "client/renderer/renderlevel/opaque_render_level.h"
#include "client/renderer/shader/opaque_mesh_shader.h"	
#include "client/renderer/core/render_resource_manager.h"
#include "client/object/component/mesh/core/mesh_component.h"
#include "client/object/component/util/camera_component.h"

namespace client_fw
{
	RenderSystem* Render::s_render_system = nullptr;

	RenderSystem::RenderSystem(const WPtr<Window>& window)
		: m_window(window)
	{
		Render::s_render_system = this;
		m_graphics_super_root_signature = CreateSPtr<GraphicsSuperRootSignature>();
		m_render_level_order = { {eRenderLevelType::kOpaque, eKindOfRenderLevel::kGraphics} };

		m_render_asset_manager = CreateUPtr<RenderResourceManager>();
	}

	RenderSystem::~RenderSystem()
	{
	}

	bool RenderSystem::Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		m_device = device;

		m_graphics_super_root_signature->Initialize(device, command_list);

		RegisterGraphicsRenderLevel<OpaqueRenderLevel>(eRenderLevelType::kOpaque);
		RegisterGraphicsShader<OpaqueMeshShader>("opaque mesh", eRenderLevelType::kOpaque);

		return true;
	}

	void RenderSystem::Shutdown()
	{
		for (const auto& [type, render_level] : m_graphics_render_levels)
			render_level->Shutdown();
		for (const auto& [name, shader] : m_graphics_shaders)
			shader->Shutdown();
		m_graphics_super_root_signature->Shutdown();
		m_device = nullptr;
		Render::s_render_system = nullptr;
	}

	void RenderSystem::Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		m_render_asset_manager->Update(device, command_list);

		for (const auto& [level_name, kind] : m_render_level_order)
		{
			switch (kind)
			{
			case eKindOfRenderLevel::kGraphics:
				m_graphics_render_levels.at(level_name)->Update(device, command_list);
				break;
			case eKindOfRenderLevel::kDeferred:
				break;
			case eKindOfRenderLevel::kCompute:
				break;
			}
		}
	}

	void RenderSystem::Draw(ID3D12GraphicsCommandList* command_list) const
	{
		m_graphics_super_root_signature->Draw(command_list);
		m_render_asset_manager->Draw(command_list);

		for (const auto& [level_name, kind] : m_render_level_order)
		{
			switch (kind)
			{
			case eKindOfRenderLevel::kGraphics:
				m_graphics_render_levels.at(level_name)->Draw(command_list, m_basic_cameras);
				break;
			case eKindOfRenderLevel::kDeferred:
				break;
			case eKindOfRenderLevel::kCompute:
				break;
			}
		}
	}

	void RenderSystem::UpdateViewport()
	{
		const auto& window = m_window.lock();
		for (const auto& camera : m_basic_cameras)
		{
			camera->UpdateViewport(static_cast<float>(window->rect.left),
				static_cast<float>(window->rect.top), static_cast<float>(window->width),
				static_cast<float>(window->height));
		}
	}

	void RenderSystem::UnregisterGraphicsShader(const std::string& shader_name, eRenderLevelType level_type)
	{
		if (m_added_shaders.find(shader_name) != m_added_shaders.cend() && 
			m_graphics_render_levels.find(level_type) != m_graphics_render_levels.cend())
		{
			auto& shader = m_graphics_shaders[shader_name];
			m_added_shaders.erase(shader_name);
			m_graphics_render_levels[level_type]->UnregisterGraphicsShader(shader);
			shader->Shutdown();
			m_graphics_shaders.erase(shader_name);
		}
	}

	bool RenderSystem::RegisterMeshComponent(const SPtr<MeshComponent>& mesh_comp, const std::string& shader_name)
	{
		m_render_asset_manager->RegisterMesh(mesh_comp->GetMesh());

		if (m_graphics_shaders.find(shader_name) == m_graphics_shaders.cend())
		{
			LOG_WARN("Could not find shader : {0}", shader_name);
			return false;
		}

		return m_graphics_shaders.at(shader_name)->RegisterMeshComponent(m_device, mesh_comp);
	}

	void RenderSystem::UnregisterMeshComponent(const SPtr<MeshComponent>& mesh_comp, const std::string& shader_name)
	{
		if (m_graphics_shaders.find(shader_name) != m_graphics_shaders.cend())
			m_graphics_shaders.at(shader_name)->UnregisterMeshComponent(mesh_comp);
	}

	bool RenderSystem::RegisterCameraComponent(const SPtr<CameraComponent>& camera_comp)
	{
		const auto& window = m_window.lock();
		switch (camera_comp->GetCameraUsage())
		{
		case eCameraUsage::kBasic:
			m_basic_cameras.push_back(camera_comp);
			camera_comp->UpdateViewport(static_cast<float>(window->rect.left), 
				static_cast<float>(window->rect.top), static_cast<float>(window->width),
				static_cast<float>(window->height));
			break;
		case eCameraUsage::kLight:
			break;
		}
		return true;
	}

	void RenderSystem::UnregisterCameraComponent(const SPtr<CameraComponent>& camera_comp)
	{
		switch (camera_comp->GetCameraUsage())
		{
		case eCameraUsage::kBasic:
		{
			auto iter = std::find(m_basic_cameras.begin(), m_basic_cameras.end(), camera_comp);
			if (iter != m_basic_cameras.end())
			{
				std::iter_swap(iter, m_basic_cameras.end() - 1);
				m_basic_cameras.pop_back();
			}
			break;
		}
		case eCameraUsage::kLight:
			break;
		}
	}			
}
