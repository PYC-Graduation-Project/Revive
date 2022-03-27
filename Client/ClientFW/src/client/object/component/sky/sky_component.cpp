#include "stdafx.h"
#include "client/object/component/sky/sky_component.h"

namespace client_fw
{
	SkyComponent::SkyComponent(eSkyType type, const std::string& name)
		: Component(name, 1000), m_sky_type(type)
	{
	}
}
