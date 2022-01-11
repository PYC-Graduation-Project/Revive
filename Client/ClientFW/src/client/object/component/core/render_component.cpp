#include "stdafx.h"
#include "client/object/component/core/render_component.h"
#include "client/renderer/core/render.h"

namespace client_fw
{
	RenderComponent::RenderComponent(const std::string& name, eRenderComponentType type, 
		const std::string& draw_shader_name, int update_order)
		: Component(name, update_order), m_render_type(type)
		, m_draw_shader_name(draw_shader_name)
	{
	}

	bool RenderComponent::RegisterToRenderSystem()
	{
		return Render::RegisterRenderComponent(shared_from_this(), m_draw_shader_name);
	}

	void RenderComponent::UnregisterFromRenderSystem()
	{
		Render::UnregisterRenderComponent(shared_from_this(), m_draw_shader_name);
	}
}
