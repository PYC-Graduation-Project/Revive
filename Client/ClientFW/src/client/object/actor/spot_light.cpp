#include "stdafx.h"
#include "client/object/actor/spot_light.h"
#include "client/object/component/light/spot_light_component.h"

namespace client_fw
{
	SpotLight::SpotLight(eMobilityState mobility, const std::string& name)
		: Actor(mobility, name)
	{
		m_spot_light_component = CreateSPtr<SpotLightComponent>();
	}

	bool SpotLight::Initialize()
	{
		bool ret = AttachComponent(m_spot_light_component);
		return ret;
	}

	void SpotLight::Shutdown()
	{
		m_spot_light_component = nullptr;
	}

	const Vec3& SpotLight::GetLightColor() const
	{
		return m_spot_light_component->GetLightColor();
	}

	void SpotLight::SetLightColor(const Vec3& color)
	{
		m_spot_light_component->SetLightColor(color);
	}

	float SpotLight::GetAttenuationRadius() const
	{
		return m_spot_light_component->GetAttenuationRadius();
	}

	void SpotLight::SetAttenuationRadius(float radius)
	{
		m_spot_light_component->SetAttenuationRadius(radius);
	}

	float SpotLight::GetConeInnerAngle() const
	{
		return m_spot_light_component->GetConeInnerAngle();
	}

	void SpotLight::SetConeInnerAngle(float degree)
	{
		m_spot_light_component->SetConeInnerAngle(degree);
	}

	float SpotLight::GetConeOuterAngle() const
	{
		return m_spot_light_component->GetConeOuterAngle();
	}

	void SpotLight::SetConeOuterAngle(float degree)
	{
		m_spot_light_component->SetConeOuterAngle(degree);
	}

	void SpotLight::SetShadowTextureSize(INT extent)
	{
		m_spot_light_component->SetShadowTextureSize(extent);
	}
}
