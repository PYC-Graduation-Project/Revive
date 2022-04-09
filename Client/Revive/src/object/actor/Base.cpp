#include "object/actor/Base.h"
#include <client/input/input.h>

namespace revive
{
	Base::Base(const std::vector<SPtr<StaticMeshComponent>>& static_mesh_components, const std::vector<SPtr<BoxComponent>>& box_components)
		:Wall(static_mesh_components,box_components)
	{
		m_mobility_state = eMobilityState::kDestructible;
	}
	bool Base::Initialize()
	{
		Wall::Initialize();
		m_hp = 5;
		//SetName("Base");
		RegisterPressedEvent("HP down", { {eKey::kB} },
			[this]()->bool {--m_hp; LOG_INFO(GetName() + "ÀÇ Ã¼·Â {0}", m_hp); return true; });
		UseUpdate();
		return true;
	}
	void Base::Update(float delta_time)
	{
		if (m_hp <= 0)
			SetActorState(eActorState::kDead);
	}
}