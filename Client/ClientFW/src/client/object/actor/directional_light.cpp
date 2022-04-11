#include "stdafx.h"
#include "client/object/actor/directional_light.h"
#include "client/object/component/light/directional_light_component.h"

namespace client_fw
{
	DirectionalLight::DirectionalLight(eMobilityState mobility, const std::string& name)
		: Actor(mobility, name)
	{
		m_light_component = CreateSPtr<DirectionalLightComponent>();
	}

	bool DirectionalLight::Initialize()
	{
		bool ret = AttachComponent(m_light_component);
		return ret;
	}

	void DirectionalLight::Shutdown()
	{
	}

	const Vec3& DirectionalLight::GetLightColor() const
	{
		return m_light_component->GetLightColor();
	}

	void DirectionalLight::SetLightColor(const Vec3& color)
	{
		m_light_component->SetLightColor(color);
	}

	const Vec3& DirectionalLight::GetLightDirection() const
	{
		return m_light_component->GetDirection();
	}

	void DirectionalLight::SetLightDirection(const Vec3& direction)
	{
		m_light_component->SetDirection(direction);
	}
}