#include <include/client_core.h>
#include <client/object/component/render/box_component.h>
#include "object/actor/weapon/axe.h"
#include "object/actor/gameplaymechanics/base.h"
#include "object/actor/character/revive_player.h"

namespace revive
{
	Axe::Axe(const std::string name)
		:Weapon("Contents/axe.obj",name)
	{
		m_rotation_offset.x = 70.f;
	}
	bool Axe::Initialize()
	{
		bool ret = Weapon::Initialize();

		ret &= AttachComponent(m_box_component);
		m_box_component->SetExtents(Vec3{ 130.f,10.f,130.f });
		m_box_component->SetLocalPosition(Vec3{ 0.f,0.f,-90.f });
		m_box_component->SetCollisionInfo(true, false, "axe", { "player hit" }, true);
		m_box_component->OnCollisionResponse([this](const SPtr<SceneComponent>& component, const SPtr<Actor>& other_actor,
			const SPtr<SceneComponent>& other_component) {
			m_box_component->SetCollisionInfo(false, false, false);
			LOG_INFO(component->GetName() + " " + other_actor->GetName() + " " + other_component->GetName());
			const auto& player = std::dynamic_pointer_cast<DefaultPlayer>(other_actor);
			if (player != nullptr)
			{
				int player_hp = player->GetHP();
				if (player_hp > 0) //서버에 플레이어 체력 전송
					player->Hit(0);
			}
			else
			{
				const auto& base = std::dynamic_pointer_cast<Base>(other_actor);
				if (base != nullptr)
				{
					int base_hp = base->GetHP();
					if (base_hp > 0) //서버에 기지 체력 전송
						base->SetHP(base_hp);
				}
			}
			LOG_INFO("충돌 부위 :" + other_component->GetName());
		});
		return ret;
	}
}

