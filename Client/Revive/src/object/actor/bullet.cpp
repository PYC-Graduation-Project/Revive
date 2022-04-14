#include <include/client_core.h>
#include <client/object/component/render/sphere_component.h>
#include "object/actor/bullet.h"

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
		return true;
	}


}