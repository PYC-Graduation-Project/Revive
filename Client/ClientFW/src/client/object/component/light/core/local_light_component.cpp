#include "stdafx.h"
#include "client/object/component/light/core/local_light_component.h"

namespace client_fw
{
	LocalLightComponent::LocalLightComponent(eLightType type,
		const std::string& name, const std::string& draw_shader_name)
		: LightComponent(type, name, draw_shader_name)
	{
	}

	void LocalLightComponent::RegisterToVisualOctree()
	{
		RenderComponent::RegisterToVisualOctree();
	}

	void LocalLightComponent::UnregisterFromVisualOctree()
	{
		RenderComponent::UnregisterFromVisualOctree();
	}
}
