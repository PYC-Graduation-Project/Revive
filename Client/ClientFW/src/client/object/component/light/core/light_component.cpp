#include "stdafx.h"
#include "client/object/component/light/core/light_component.h"
#include "client/renderer/core/render.h"

namespace client_fw
{
	LightComponent::LightComponent(eLightType type, const std::string& name)
		: SceneComponent(name, 10)
		, m_light_type(type)
	{
	}

	bool LightComponent::Initialize()
	{
		return RegisterToRenderSystem();
	}

	void LightComponent::Shutdown()
	{
		UnregisterFromRenderSystem();
	}

	bool LightComponent::RegisterToRenderSystem()
	{
		return Render::RegisterLightComponent(SharedFromThis());
	}

	void LightComponent::UnregisterFromRenderSystem()
	{
		Render::UnregisterLightComponent(SharedFromThis());
	}

	SPtr<LightComponent> LightComponent::SharedFromThis()
	{
		return std::static_pointer_cast<LightComponent>(shared_from_this());
	}
}
