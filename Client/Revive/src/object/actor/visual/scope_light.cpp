#include <include/client_core.h>
#include <client/object/component/mesh/static_mesh_component.h>
#include <client/object/component/light/spot_light_component.h>

#include "scope_light.h"

namespace revive
{
	ScopeLight::ScopeLight()
		: StaticMeshActor(eMobilityState::kStatic, "Contents/Visual/scope_light.obj")
	{
		m_spot_light_component = CreateSPtr<SpotLightComponent>();
	}

	bool ScopeLight::Initialize()
	{
		SetScale(4.f);

		bool ret = StaticMeshActor::Initialize();

		m_spot_light_component->SetLightColor(Vec3(1.0f, 1.0f, 1.0f) * 50000.0f);
		m_spot_light_component->SetLocalPosition(Vec3(7.5f, 30.0f, 0.0f));
		m_spot_light_component->SetLocalRotation(math::ToRadian(-45.0f), 0.0f, 0.0f);
		m_spot_light_component->SetAttenuationRadius(1500.0f);
		m_spot_light_component->SetConeInnerAngle(20.0f);
		m_spot_light_component->SetConeOuterAngle(40.0f);
		ret &= AttachComponent(m_spot_light_component);

		return ret;
	}

	void ScopeLight::Shutdown()
	{
	}

	void ScopeLight::Update(float delta_time)
	{
	}

}