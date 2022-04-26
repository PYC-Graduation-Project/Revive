#include <include/client_core.h>
#include <client/input/input.h>
#include <client/object/component/render/box_component.h>
#include "revive_server/message/message_event_info.h"
#include "object/actor/gameplaymechanics/base.h"

namespace revive
{
	Base::Base(const std::vector<SPtr<StaticMeshComponent>>& static_mesh_components, const std::vector<SPtr<BoxComponent>>& box_components)
		:Wall(static_mesh_components,box_components)
	{
		m_mobility_state = eMobilityState::kDestructible;
	}
	bool Base::Initialize()
	{
		bool ret = true;
		for (auto& static_mesh_component : m_static_mesh_components)
			ret &= AttachComponent(static_mesh_component);

		for (auto& box_component : m_box_components)
		{
			box_component->SetCollisionInfo(true, true, "base", { "player","enemy","stone", "enemy agro" }, false);
			ret &= AttachComponent(box_component);
		}

		m_hp = 5;

		RegisterPressedEvent("HP down", { {eKey::kB} },
			[this]()->bool {--m_hp; LOG_INFO(GetName() + "ÀÇ Ã¼·Â {0}", m_hp); return true; });

		UseUpdate();

		return ret;
	}

	void Base::Update(float delta_time)
	{
		if (m_hp <= 0)
			SetActorState(eActorState::kDead);
	}
	void Base::ExecuteMessageFromServer(const SPtr<MessageEventInfo>& message)
	{
		switch (message->GetEventID())
		{
		case HashCode("base hp change"):
		{
			auto msg = std::static_pointer_cast<BaseHpChangeEventInfo>(message);
			m_hp = msg->GetBaseHp();
			break;
		}

		}
	}
}