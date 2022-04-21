#include <include/client_core.h>
#include <client/object/component/render/sphere_component.h>
#include <client/object/component/mesh/static_mesh_component.h>
#include "object/actor/projectile/bullet.h"

namespace revive
{
	Bullet::Bullet(const std::string& name)
		:Projectile(name)
	{
	}

	bool Bullet::Initialize()
	{
		Projectile::Initialize();
		SetInitialSpeed(5000.f);

		bool ret = m_static_mesh_component->SetMesh("../Contents/sphere.obj");
		ret &= AttachComponent(m_static_mesh_component);
		m_static_mesh_component->SetLocalScale(0.1f);

		return ret;
	}


}