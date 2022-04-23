#pragma once
#include "client/object/component/light/core/light_component.h"

namespace client_fw
{
	class DirectionalLightComponent : public LightComponent
	{
	public:
		DirectionalLightComponent();
		virtual ~DirectionalLightComponent() = default;
	};
}


