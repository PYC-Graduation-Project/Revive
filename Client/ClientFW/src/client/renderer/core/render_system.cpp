#include "stdafx.h"
#include "client/renderer/core/render.h"
#include "client/renderer/core/render_system.h"
#include "client/renderer/renderlevel/default_render_level.h"
#include "client/renderer/shader/default_shader.h"	
#include "client/object/component/core/render_component.h"
#include "client/object/component/mesh/core/mesh_component.h"
#include "client/asset/mesh/mesh.h"

namespace client_fw
{
	RenderSystem* Render::s_render_system = nullptr;

	RenderSystem::RenderSystem()
	{
		Render::s_render_system = this;
		m_render_level_order = { "default" };
	}

	bool RenderSystem::Initialize(ID3D12Device* device)
	{
		m_device = device;

		RegisterGraphicsRenderLevel<DefaultRenderLevel>("default");
		RegisterGraphicsShader<DefaultShader>("default", "default");

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

		for (const auto& level_name : m_render_level_order)
		{
			//compute생각
			m_graphics_render_levels.at(level_name)->Update(device, command_list);
		}
	}

	void RenderSystem::Draw(ID3D12GraphicsCommandList* command_list)
	{
		for (const auto& level_name : m_render_level_order)
		{
			//compute생각
			m_graphics_render_levels.at(level_name)->Draw(command_list);
		}
	}

	void RenderSystem::UnregisterGraphicsRenderLevel(const std::string& level_name)
	{
		if (m_added_render_levels.find(level_name) != m_added_render_levels.cend())
		{
			m_added_render_levels.erase(level_name);
			m_graphics_render_levels[level_name]->Shutdown();
			m_graphics_render_levels.erase(level_name);
			auto iter = find(m_render_level_order.begin(), m_render_level_order.end(), level_name);
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

	bool RenderSystem::RegisterRenderComponent(const SPtr<RenderComponent>& render_comp,
		eRenderComponentType comp_type, const std::string& shader_name)
	{
		return false;
	}

	void RenderSystem::UnregisterRenderComponent(const SPtr<RenderComponent>& render_comp, 
		eRenderComponentType comp_type,	const std::string& shader_name)
	{
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

	void RenderSystem::AddRenderLevelOrder(const std::string& name, const std::string& front_render_level_name)
	{
		if (front_render_level_name.empty())
		{
			m_render_level_order.insert(m_render_level_order.begin(), name);
		}
		else
		{
			auto iter = std::find(m_render_level_order.begin(), m_render_level_order.end(), front_render_level_name);
			if (iter != m_render_level_order.end())
				m_render_level_order.insert(iter + 1, name);
			else
				LOG_ERROR("Could not find {0}", front_render_level_name);
		}
	}
			
}
