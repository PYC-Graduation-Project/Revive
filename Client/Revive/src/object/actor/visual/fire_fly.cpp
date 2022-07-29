#include <include/client_core.h>
#include <client/object/component/mesh/static_mesh_component.h>
#include <client/object/component/light/point_light_component.h>
#include <client/object/component/util/simple_movement_component.h>

#include "fire_fly.h"

namespace revive
{
	FireFly::FireFly()
		: StaticMeshActor(eMobilityState::kStatic, "Contents/visual/light_sphere.obj")
	{
		m_point_light_component = CreateSPtr<PointLightComponent>();
		m_simple_movement_component = CreateSPtr<SimpleMovementComponent>();
	}

	bool FireFly::Initialize()
	{
		bool ret = StaticMeshActor::Initialize();
		
		m_point_light_component->SetAttenuationRadius(20.0f * GetScale().x);
		m_point_light_component->DisableShadow();
		ret &= AttachComponent(m_point_light_component);

		return ret;
	}

	void FireFly::Shutdown()
	{
		StaticMeshActor::Shutdown();
	}

	void FireFly::Update(float delta_time)
	{
	}
}