#include "stdafx.h"
#include "client/renderer/core/render.h"
#include "client/renderer/core/render_system.h"
#include "client/renderer/renderlevel/opaque_render_level.h"
#include "client/renderer/shader/opaque_mesh_shader.h"	
#include "client/object/component/mesh/core/mesh_component.h"
#include "client/object/component/util/camera_component.h"
#include "client/asset/mesh/mesh.h"

namespace client_fw
{
	RenderSystem* Render::s_render_system = nullptr;

	RenderSystem::RenderSystem()
	{
		Render::s_render_system = this;
		m_render_level_order = { {"opaque", eKindOfRenderLevel::kGraphics} };
	}

	bool RenderSystem::Initialize(ID3D12Device* device)
	{
		m_device = device;

		RegisterGraphicsRenderLevel<OpaqueRenderLevel>("opaque");
		RegisterGraphicsShader<OpaqueMeshShader>("opaque mesh", "opaque");

		return true;
	}

	void RenderSystem::Shutdown()
	{
		for (const auto& [type, render_level] : m_graphics_render_levels)
			render_level->Shutdown();
		for (const auto& [name, shader] : m_graphics_shaders)
			shader->Shutdown();
		m_device = nullptr;
		Render::s_render_system = nullptr;
	}

	void RenderSystem::Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		for (const auto& mesh : m_ready_meshes)
		{
			mesh->Initialize(device, command_list);
			LOG_INFO(mesh->GetPath());
		}
		m_ready_meshes.clear();

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

	void RenderSystem::Draw(ID3D12GraphicsCommandList* command_list)
	{
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

	void RenderSystem::UnregisterGraphicsRenderLevel(const std::string& level_name)
	{
		if (m_added_render_levels.find(level_name) != m_added_render_levels.cend())
		{
			m_added_render_levels.erase(level_name);
			m_graphics_render_levels[level_name]->Shutdown();
			m_graphics_render_levels.erase(level_name);
			auto iter = find(m_render_level_order.begin(), m_render_level_order.end(), 
				std::make_pair(level_name, eKindOfRenderLevel::kGraphics));
			m_render_level_order.erase(iter);
		}
	}

	void RenderSystem::UnregisterGraphicsShader(const std::string& shader_name, const std::string& level_name)
	{
		if (m_added_shaders.find(shader_name) != m_added_shaders.cend() && 
			m_graphics_render_levels.find(level_name) != m_graphics_render_levels.cend())
		{
			auto& shader = m_graphics_shaders[shader_name];
			m_added_shaders.erase(shader_name);
			m_graphics_render_levels[level_name]->UnregisterGraphicsShader(shader);
			shader->Shutdown();
			m_graphics_shaders.erase(shader_name);
		}
	}

	bool RenderSystem::RegisterMeshComponent(const SPtr<MeshComponent>& mesh_comp, const std::string& shader_name)
	{
		if (m_initialized_assets.find(mesh_comp->GetMesh()->GetPath()) == m_initialized_assets.cend())
		{
			m_initialized_assets.insert(mesh_comp->GetMesh()->GetPath());
			m_ready_meshes.push_back(mesh_comp->GetMesh());
		}

		if (m_graphics_shaders.find(shader_name) == m_graphics_shaders.cend())
		{
			LOG_WARN("Could not find shader : {0}", shader_name);
			return false;
		}

		return m_graphics_shaders.at(shader_name)->RegisterMeshComponent(mesh_comp);
	}

	void RenderSystem::UnregisterMeshComponent(const SPtr<MeshComponent>& mesh_comp, const std::string& shader_name)
	{
		if (m_graphics_shaders.find(shader_name) != m_graphics_shaders.cend())
			m_graphics_shaders.at(shader_name)->UnregisterMeshComponent(mesh_comp);
	}

	bool RenderSystem::RegisterCameraComponent(const SPtr<CameraComponent>& camera_comp)
	{
		switch (camera_comp->GetCameraUsage())
		{
		case eCameraUsage::kBasic:
			m_basic_cameras.push_back(camera_comp);
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

	void RenderSystem::AddRenderLevelOrder(const std::string& name, const std::string& front_render_level_name, eKindOfRenderLevel kind)
	{
		if (front_render_level_name.empty())
		{
			m_render_level_order.insert(m_render_level_order.begin(), { name, kind });
		}
		else
		{
			auto iter = std::find(m_render_level_order.begin(), m_render_level_order.end(), std::pair{ front_render_level_name, kind });
			if (iter != m_render_level_order.end())
				m_render_level_order.insert(iter + 1, { name, kind });
			else
				LOG_ERROR("Could not find {0}", front_render_level_name);
		}
	}
			
}
