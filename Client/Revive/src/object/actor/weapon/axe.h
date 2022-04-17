#pragma once
#include "object/actor/weapon/weapon.h"

namespace revive
{
	class Axe : public Weapon
	{
	public:
		Axe(const std::string name = "axe");
		virtual ~Axe() = default;

	};
}