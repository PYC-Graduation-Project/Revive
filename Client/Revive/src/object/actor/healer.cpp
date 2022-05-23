#include <include/client_core.h>
#include <include/dx12_shader_fw.h>
#include <client/object/component/mesh/static_mesh_component.h>
#include <client/object/component/render/box_component.h>
#include <client/object/component/light/point_light_component.h>
#include <client/asset/mesh/mesh.h>
#include <client/asset/material/material.h>

#include "healer.h"
//#include "object/level/game_play_level.h"
//#include "object/ui/util/debugging_ui_layer.h"
#include "object/actor/character/revive_player.h"

#include "revive_server/message/message_event_info.h"


namespace revive
{
	Healer::Healer()
		: Actor(eMobilityState::kStatic, "Healer")
	{
		m_base_mesh = CreateSPtr<StaticMeshComponent>();
		m_jewelry_mesh = CreateSPtr<StaticMeshComponent>();
		m_collision_box = CreateSPtr<BoxComponent>();
		m_heal_box = CreateSPtr<BoxComponent>();
	}

	bool Healer::Initialize()
	{
		SetRotation(0.0f, math::ToRadian(180.0f), 0.0f);

		bool ret = true;
		ret &= m_base_mesh->SetMesh("Contents/visual/heal_base.obj");
		ret &= m_jewelry_mesh->SetMesh("Contents/visual/heal_jewelry.obj");

		for (const auto& material : m_jewelry_mesh->GetStaticMesh()->GetMaterials(0))
		{
			material->SetMetallic(1.0f);
			material->SetRoughness(0.2f);
		}

		m_collision_box->SetExtents(Vec3{ 250.f,400.f,300.f } / 2);
		m_collision_box->SetLocalPosition(Vec3{ 0.f,200.f,-25.f });
		m_collision_box->SetCollisionInfo(true, true, "wall", { "player" ,"player camera" }, true);

		//아래 내용은 서버에서 처리한다.
		m_heal_box->SetExtents(Vec3{ 850.f,300.f,850.f } / 2);
		m_heal_box->SetLocalPosition(Vec3{ 0.f,150.f,0.f });
		m_heal_box->SetCollisionInfo(true, false, "healer", { "player" },true);
		m_heal_box->OnCollisionResponse([this](const SPtr<SceneComponent>& component, const SPtr<Actor>& other_actor,
			const SPtr<SceneComponent>& other_component)
		{
			const auto& player = std::dynamic_pointer_cast<DefaultPlayer>(other_actor);
			if (player)
			{
				/*const auto& game_level = std::dynamic_pointer_cast<GamePlayLevel>(LevelManager::GetLevelManager().GetCurrentLevel());
				game_level->LogInfoUI(L"우왕 힐존이다!");*/
				//LOG_INFO("힐존에 플레이어가 들어와있네!");
				if (player->GetHP() != player->GetMaxHP())
				{
				}
				//쿨타임을 넣고 천천히 차게한다 || 매프레임 HP가 오르나 매우 작은 값을 올려서 서서히 차게한다.
			}
		});
		ret &= AttachComponent(m_base_mesh);
		ret &= AttachComponent(m_jewelry_mesh);
		ret &= AttachComponent(m_collision_box);
		//ret &= AttachComponent(m_heal_box);
		return ret;
	}

	void Healer::Shutdown()
	{
	}

	void Healer::Update(float delta_time)
	{
	}

	void Healer::ExecuteMessageFromServer(const SPtr<MessageEventInfo>& message)
	{
		switch (message->GetEventID())
		{
		case HashCode(""):
		{
		}
		}
	}

}