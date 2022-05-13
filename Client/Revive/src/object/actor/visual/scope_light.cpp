#include <include/client_core.h>
#include <client/object/component/mesh/static_mesh_component.h>
#include <client/object/component/light/spot_light_component.h>
#include <client/object/component/light/point_light_component.h>

#include "scope_light.h"

namespace revive
{
	ScopeLight::ScopeLight()
		: StaticMeshActor(eMobilityState::kStatic, "Contents/Visual/scope_light.obj")
	{
		m_spot_light_component = CreateSPtr<SpotLightComponent>();
		m_point_light_component = CreateSPtr<PointLightComponent>();
	}

	bool ScopeLight::Initialize()
	{
		SetScale(0.5f);

		bool ret = StaticMeshActor::Initialize();

		m_spot_light_component->SetLightColor(Vec3(1.0f, 1.0f, 1.0f) * 400000.0f);
		m_spot_light_component->SetLocalPosition(Vec3(0.0f, 95.0f, -170.0f));
		m_spot_light_component->SetLocalRotation(math::ToRadian(25.0f), math::ToRadian(180.0f), 0.0f);
		m_spot_light_component->SetAttenuationRadius(2000.0f);
		m_spot_light_component->SetConeOuterAngle(25.0f);
		ret &= AttachComponent(m_spot_light_component);

		m_point_light_component->SetLightColor(Vec3(1.0f, 1.0f, 1.0f) * 400000.0f);
		m_point_light_component->SetLocalPosition(Vec3(0.0f, 95.0f, -170.0f));
		m_point_light_component->SetAttenuationRadius(40.0f);
		m_point_light_component->DisableShadow();
		ret &= AttachComponent(m_point_light_component);

		return ret;
	}

	void ScopeLight::Shutdown()
	{
	}

	void ScopeLight::Update(float delta_time)
	{

	}

}