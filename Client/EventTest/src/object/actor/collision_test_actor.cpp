#include <include/client_core.h>
#include <client/input/input.h>
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
		m_static_mesh_component->SetCollisionInfo(true, false, true);

		auto sphere = CreateSPtr<SphereComponent>(100.0f);
		//auto sphere = CreateSPtr<BoxComponent>(Vec3(100.0f, 100.0f, 100.0f));
		ret &= AttachComponent(sphere);
		UseUpdate();

		RegisterPressedEvent("enable collision", { EventKeyInfo{eKey::kO} },
			[this]()->bool {
				m_static_mesh_component->SetCollisionInfo(true, false, true);
				return true;
			});

		m_static_mesh_component->OnCollisionResponse([this](const SPtr<SceneComponent>& component, const SPtr<Actor>& other_actor,
			const SPtr<SceneComponent>& other_component) {
				//LOG_INFO("충돌했다.");
				component->SetCollisionInfo(false, false, false);
			});

		return ret;
	}

	void CollisionTestActor::Shutdown()
	{
		StaticMeshActor::Shutdown();
	}

	void CollisionTestActor::Update(float delta_time)
	{
	/*	m_time += delta_time;

		if (m_time >= 1.0f)
		{
			m_time -= 1.0f;

			m_is_col = !m_is_col;
			m_static_mesh_component->SetCollisionInfo(m_is_col, false, false);
		}*/
	}
}