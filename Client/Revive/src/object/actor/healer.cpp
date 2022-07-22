#include <include/client_core.h>
#include <include/dx12_shader_fw.h>
#include <client/object/component/mesh/static_mesh_component.h>
#include <client/object/component/render/box_component.h>
#include <client/object/component/light/point_light_component.h>
#include <client/asset/mesh/mesh.h>
#include <client/asset/material/material.h>

#include "healer.h"
//#include "object/level/game_play_level.h"
#include "object/actor/character/revive_player.h"
#include "object/effect/heal_effect.h"

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
		m_heal_box->SetCollisionInfo(true, false, "healer", { "player" ,"player hit"}, true);
		m_heal_box->OnCollisionResponse([this](const SPtr<SceneComponent>& component, const SPtr<Actor>& other_actor,
			const SPtr<SceneComponent>& other_component)
		{
			const auto& player = std::dynamic_pointer_cast<DefaultPlayer>(other_actor);
			if (player) //플레이어가 들어오면
			{
				LOG_INFO("dd");
				auto heal_effect = m_heal_effect.lock();
				heal_effect->SetCleanTime(0.f);

				//이펙트 생성 매번 Spawn하는 방식
				//만약 스폰방식이 성능에 안좋으면, 미리 스폰해놓고
				//힐박스안에 들어오면 파티클을 움직이는 방식 시도해보기
				heal_effect->CreateEffect();
				
			}
			
		});

		if(m_heal_effect.expired() == false)
			m_heal_effect.lock()->RegisterBox(m_heal_box);
		
		ret &= AttachComponent(m_base_mesh);
		ret &= AttachComponent(m_jewelry_mesh);
		ret &= AttachComponent(m_collision_box);
		ret &= AttachComponent(m_heal_box);
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