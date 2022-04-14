#pragma once
#include "object/actor/projectile/projectile.h"

namespace revive
{
	class Bullet : public Projectile
	{
	public:
		Bullet(const std::string& name = "Bullet");
		virtual ~Bullet() = default;

		virtual bool Initialize() override;
	};
}