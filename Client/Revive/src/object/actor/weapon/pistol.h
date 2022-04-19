#pragma once
#include "object/actor/weapon/weapon.h"

namespace revive
{
	class Pistol : public Weapon
	{
	public:
		Pistol(const std::string name = "pistol");
		virtual ~Pistol() = default;

	};
}