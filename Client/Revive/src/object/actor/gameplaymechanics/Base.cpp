#include <include/client_core.h>
#include <client/input/input.h>
#include <client/object/component/render/box_component.h>
#include <client/event/packetevent/packet_helper.h>

#include "revive_server/message/message_event_info.h"
#include "object/actor/gameplaymechanics/base.h"

namespace revive
{
	Base::Base()
		:VisualActor()
	{
		m_mobility_state = eMobilityState::kDestructible;
		auto static_mesh_component = CreateSPtr<StaticMeshComponent>();
		static_mesh_component->SetMesh("Contents/towerSquare.obj");
		static_mesh_component->SetLocalPosition(Vec3(0.f, -300.f, -20.f));
		static_mesh_component->SetLocalScale(30.f);
		m_static_mesh_components.push_back(static_mesh_component);
		auto box_component = CreateSPtr<BoxComponent>();
		box_component->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
		box_component->SetExtents(Vec3(150.f, 300.f, 150.f));
		m_box_components.push_back(box_component);
		SetPosition(Vec3(2400.f, 600.f, 2850.f));
	}
	Base::Base(const std::vector<SPtr<StaticMeshComponent>>& static_mesh_components, const std::vector<SPtr<BoxComponent>>& box_components)
		:VisualActor(static_mesh_components,box_components)
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
			box_component->SetCollisionInfo(true, true, "base", { "player","enemy","stone", "enemy agro","player camera"}, false);
			ret &= AttachComponent(box_component);
		}


		RegisterPressedEvent("HP down", { {eKey::kB} },
			[this]()->bool {--m_hp; LOG_INFO(GetName() + "ÀÇ Ã¼·Â {0}", m_hp); return true; });

		UseUpdate();
		return ret;
	}

	void Base::Update(float delta_time)
	{
		/*if (m_hp <= 0)
			SetActorState(eActorState::kDead);*/
	}
	void Base::Shutdown()
	{
		m_changed_hp_function = nullptr;
		if(IsConnectedToServer())
			PacketHelper::DisconnectActorFromServer(BASE_ID);

	}
	void Base::ExecuteMessageFromServer(const SPtr<MessageEventInfo>& message)
	{
		switch (message->GetEventID())
		{
		case HashCode("base hp change"):
		{
			if (m_hp > 0)
			{
				auto msg = std::static_pointer_cast<BaseHpChangeEventInfo>(message);
				float hp = msg->GetBaseHp();
				if (hp < 0) hp = 0;

				if (hp == 0)
					PacketHelper::DisconnectActorFromServer(BASE_ID);
				m_hp = hp;
				if (m_changed_hp_function != nullptr)
					m_changed_hp_function(m_hp, m_max_hp);
			}
			
			break;
		}

		}
	}
}