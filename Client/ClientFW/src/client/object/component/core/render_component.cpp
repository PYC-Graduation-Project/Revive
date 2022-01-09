#include "stdafx.h"
#include "client/object/component/core/render_component.h"
#include "client/renderer/core/render.h"

namespace client_fw
{
	RenderComponent::RenderComponent(const std::string& name, eRenderComponentType type, int update_order)
		: Component(name, update_order), m_render_type(type)
	{
	}

	void RenderComponent::RegisterToRenderSystem(const std::string& shader_name)
	{
		Render::RegisterRenderComponent(shared_from_this(), shader_name);
	}

	void RenderComponent::RegisterToRenderSystem(eShaderType shader_type)
	{
		Render::RegisterRenderComponent(shared_from_this(), shader_type);
	}

	void RenderComponent::UnregisterFromRenderSystem(const std::string& shader_name)
	{
		Render::UnregisterRenderComponent(shared_from_this(), shader_name);
	}

	void RenderComponent::UnregisterFromRenderSystem(eShaderType shader_type)
	{
		Render::UnregisterRenderComponent(shared_from_this(), shader_type);
	}

}
