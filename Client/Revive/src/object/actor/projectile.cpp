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

		m_projectile_movement_component->SetInitialSpeed(10000.0f);
		m_projectile_movement_component->SetProjectileGravityScale(0.0f);

		ret &= AttachComponent(m_projectile_movement_component);
		
		m_static_mesh_component->SetMesh("../Contents/Sphere.obj");
		ret &= AttachComponent(m_static_mesh_component);

		ret &= AttachComponent(m_sphere_component);

		SetPosition(Vec3{ 2400.0f,400.0f,4000.0f });
		return ret;
	}

	void Projectile::Shutdown()
	{
		m_static_mesh_component = nullptr;
		m_projectile_movement_component = nullptr;
		m_sphere_component = nullptr;
	}


}