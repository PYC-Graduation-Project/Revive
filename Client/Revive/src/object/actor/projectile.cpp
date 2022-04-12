#include <include/client_core.h>
#include <client/object/component/mesh/static_mesh_component.h>
#include <client/object/component/render/sphere_component.h>
#include "object/component/projectile_movement.h"
#include "object/actor/projectile.h"

namespace revive
{
	Projectile::Projectile(const std::string& name)
		:Actor(eMobilityState::kMovable,name)
	{
		m_static_mesh_component = CreateSPtr<StaticMeshComponent>();
		m_projectile_movement_component = CreateSPtr<ProjectileMovementComponent>();
		m_sphere_component = CreateSPtr<SphereComponent>(32.f);
	}

	bool Projectile::Initialize()
	{
		bool ret = true;

		m_projectile_movement_component->SetInitialSpeed(1000.0f);
		m_projectile_movement_component->SetProjectileGravityScale(0.0f);

		ret &= AttachComponent(m_projectile_movement_component);
		
		m_static_mesh_component->SetMesh("../Contents/Sphere.obj");
		m_static_mesh_component->SetLocalScale(0.1f);
		ret &= AttachComponent(m_static_mesh_component);

		ret &= AttachComponent(m_sphere_component);
		m_life_span = 1.0f;
		return ret;
	}

	void Projectile::Shutdown()
	{
		m_static_mesh_component = nullptr;
		m_projectile_movement_component = nullptr;
		m_sphere_component = nullptr;
	}

	void Projectile::Update(float delta_time)
	{
		m_age += delta_time;

		if (m_life_span > 0.f)
		{
			if (m_age >= m_life_span)
				this->SetActorState(eActorState::kDead);
		}
	}

	void Projectile::SetVelocity(const Vec3& velocity)
	{
		m_projectile_movement_component->SetVelocity(velocity);
	}

	void Projectile::SetCollisionInfo(bool is_collision, const std::string collision_type, const std::string collisionable_types, bool genrate_collision_event)
	{
		m_sphere_component->SetCollisionInfo(is_collision, false, collision_type.c_str(), { collisionable_types.c_str() }, genrate_collision_event);
	}

	void Projectile::SetOnCollisionResponse(const std::function<void(const SPtr<SceneComponent>& comp, const SPtr<Actor>& other_actor, const SPtr<SceneComponent>& other_comp)>& function)
	{
		m_sphere_component->OnCollisionResponse(function);
	}

	void Projectile::SetBlockingSphereRadius(float radius)
	{
		m_sphere_component->SetExtents(radius);
	}

	

}