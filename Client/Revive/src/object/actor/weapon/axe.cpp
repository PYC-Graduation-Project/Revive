#include <include/client_core.h>
#include "object/actor/weapon/axe.h"

namespace revive
{
	Axe::Axe(const std::string name)
		:Weapon("Contents/axe.obj",name)
	{
		m_rotation_offset.x = 70.f;
	}
}

