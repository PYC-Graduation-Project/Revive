#include <include/client_core.h>
#include "object/actor/stone.h"

namespace revive
{
	Stone::Stone(const std::string& name)
		:Projectile(name)
	{
	}
	bool Stone::Initialize()
	{
		bool ret = Projectile::Initialize();

		SetGravityScale(10.0f);
		return ret;
	}

	void Stone::Update(float delta_time)
	{
		Projectile::Update(delta_time);
		Vec3 current_position = GetPosition();
		if (current_position.y <= 300)current_position.y = 300.f + GetSphereRadius();
		SetPosition(current_position);
	}

	
}