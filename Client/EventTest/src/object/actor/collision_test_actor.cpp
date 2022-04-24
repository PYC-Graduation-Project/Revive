#include <include/client_core.h>
#include <client/object/component/mesh/static_mesh_component.h>
#include <client/object/component/render/sphere_component.h>
#include <client/object/component/render/box_component.h>
#include "collision_test_actor.h"

namespace event_test
{
	CollisionTestActor::CollisionTestActor()
		: StaticMeshActor(eMobilityState::kStatic, "../Contents/sphere.obj")
	{
	}

	bool CollisionTestActor::Initialize()
	{
		bool ret = StaticMeshActor::Initialize();
		m_static_mesh_component->SetCollisionInfo(true, false, false);

		auto sphere = CreateSPtr<SphereComponent>(100.0f);
		//auto sphere = CreateSPtr<BoxComponent>(Vec3(100.0f, 100.0f, 100.0f));
		ret &= AttachComponent(sphere);
		UseUpdate();

		return ret;
	}

	void CollisionTestActor::Shutdown()
	{
		StaticMeshActor::Shutdown();
	}

	void CollisionTestActor::Update(float delta_time)
	{
		m_time += delta_time;

		if (m_time >= 1.0f)
		{
			m_time -= 1.0f;

			m_is_col = !m_is_col;
			m_static_mesh_component->SetCollisionInfo(m_is_col, false, false);
		}
	}
}