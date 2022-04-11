#include "stdafx.h"
#include "client/object/component/light/directional_light_component.h"
#include "client/renderer/core/render.h"

namespace client_fw
{
	DirectionalLightComponent::DirectionalLightComponent()
		: LightComponent(eLightType::kDirectional, "directional light component",
			Render::ConvertShaderType(eShaderType::kDeferred))
	{
	}
}
