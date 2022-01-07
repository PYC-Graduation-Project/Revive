#include "stdafx.h"
#include "client/renderer/core/render.h"
#include "client/renderer/core/render_system.h"
#include "client/renderer/renderlevel/default_render_level.h"
#include "client/renderer/shader/default_shader.h"	


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

		RegisterRenderLevel<DefaultRenderLevel>("default");
		RegisterShader<DefaultShader>("default", "default");

		return true;
	}

	void RenderSystem::Shutdown()
	{
		for (const auto& [type, render_level] : m_render_levels)
			render_level->Shutdown();
		for (const auto& [name, shader] : m_shaders)
			shader->Shutdown();
		m_device = nullptr;
		Render::s_render_system = nullptr;
	}

	void RenderSystem::Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		for (const auto& level_name : m_render_level_order)
			m_render_levels[level_name]->Update(device, command_list);
	}

	void RenderSystem::Draw(ID3D12GraphicsCommandList* command_list)
	{
		for (const auto& level_name : m_render_level_order)
			m_render_levels[level_name]->Draw(command_list);
	}

	void RenderSystem::AddRenderLevelOrder(const std::string& name, const std::string& front_render_level_name)
	{
		if (front_render_level_name.empty())
		{
			m_render_level_order.insert(m_render_level_order.begin(), name);
		}
		else
		{
			auto iter = std::find(m_render_level_order.begin(), m_render_level_order.end(), name);
			if (iter != m_render_level_order.end())
				m_render_level_order.insert(iter + 1, name);
			else
				LOG_ERROR("Could not find {0}", front_render_level_name);
		}
	}
			
}
