#include "stdafx.h"
#include "client/object/actor/point_light.h"
#include "client/object/component/light/point_light_component.h"

namespace client_fw
{
	PointLight::PointLight(eMobilityState mobility, const std::string& name)
		: Actor(mobility, name)
	{
		m_point_light_component = CreateSPtr<PointLightComponent>();
	}

	bool PointLight::Initialize()
	{
		bool ret = AttachComponent(m_point_light_component);
		return ret;
	}

	void PointLight::Shutdown()
	{
		m_point_light_component = nullptr;
	}

	const Vec3& PointLight::GetLightColor() const
	{
		return m_point_light_component->GetLightColor();
	}

	void PointLight::SetLightColor(const Vec3& color)
	{
		m_point_light_component->SetLightColor(color);
	}

	float PointLight::GetAttenuationRadius() const
	{
		return m_point_light_component->GetAttenuationRadius();
	}

	void PointLight::SetAttenuationRadius(float radius)
	{
		m_point_light_component->SetAttenuationRadius(radius);
	}
}
