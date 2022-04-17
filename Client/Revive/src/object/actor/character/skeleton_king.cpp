#include <include/client_core.h>
#include <client/object/component/mesh/skeletal_mesh_component.h>
#include <client/object/component/render/sphere_component.h>
#include <client/object/component/render/box_component.h>
#include "client/input/input.h"
#include "object/actor/character/revive_player.h"
#include "object/actor/character/skeleton_king.h"

namespace revive
{
	SkeletonKing::SkeletonKing(const std::string& name)
		:Enemy("Contents/skeleton_king.rev", name)
	{
		m_weapon_collision_box = CreateSPtr<BoxComponent>();
	}
	bool SkeletonKing::Initialize()
	{
		bool ret = true;

		ret &= Enemy::Initialize();
		ret &= AttachComponent(m_skeletal_mesh_component);
		m_skeletal_mesh_component->SetLocalRotation(math::ToRadian(-90.0f), 0.0f, 0.0f);
		m_skeletal_mesh_component->AddNotify("death end", "death", 82,
			[this]() { m_is_disappearing = true;  });
		m_skeletal_mesh_component->AddNotify("hit end", "hit", 14,
			[this]() { m_skeletal_mesh_component->SetAnimation("idle");  });
		m_skeletal_mesh_component->AddNotify("attack end", "attack", 50,
			[this]() { m_is_attacking = false; m_skeletal_mesh_component->SetAnimation("idle"); /*공격 후에 재생할 애니메이션*/});

		ret &= SetCollisionComponent();
		
		if (Input::RegisterPressedEvent(m_name + " Test", { {eKey::kT} },
			[this]()->bool { m_skeletal_mesh_component->SetAnimation(m_animation_name[m_animation_select_num++]);
		if (m_animation_select_num >= m_animation_name.size()) m_animation_select_num = 0; return true; }
			, true, eInputOwnerType::kActor))
			RegisterInputEvent(m_name + " Test");
		
		m_hp = 20;
		//SetPosition(Vec3{ 2000.0f,300.0f,4000.0f });
		SetScale(0.6f);
		return ret;
	}
	void SkeletonKing::Shutdown()
	{
		Enemy::Shutdown();
	}
	bool SkeletonKing::SetCollisionComponent()
	{
		bool ret = true;
		m_blocking_sphere->SetExtents(80.0f);
		m_blocking_sphere->SetLocalPosition(Vec3{ 0.0f,m_blocking_sphere->GetExtents().y,0.0f }); //x,y,z 상관없음
		m_blocking_sphere->SetCollisionInfo(false, false, "enemy", { "wall" }, true);
		m_blocking_sphere->OnCollisionResponse([this](const SPtr<SceneComponent>& component, const SPtr<Actor>& other_actor,
			const SPtr<SceneComponent>& other_component) {
			FixYPosition();
			LOG_INFO(GetName() + ": Sphere component {0} Enemy Position {1} Extents {2}", m_blocking_sphere->GetWorldPosition(), this->GetPosition(), m_blocking_sphere->GetExtents());
		});

		ret &= AttachComponent(m_blocking_sphere);

		//멀티에서만 사용
		m_blocking_box->SetExtents(Vec3{50.f,200.f,50.f});
		m_blocking_box->SetLocalPosition(Vec3{ 0.0f,m_blocking_box->GetExtents().y,0.0f });
		m_blocking_box->SetCollisionInfo(true, false, "enemy", { "wall" }, true);
		m_blocking_box->OnCollisionResponse([this](const SPtr<SceneComponent>& component, const SPtr<Actor>& other_actor,
			const SPtr<SceneComponent>& other_component) {
			//LOG_INFO(GetName() + ": Box component {0} Enemy Position {1} Extents {2}", m_blocking_box->GetWorldPosition(), this->GetPosition(), m_blocking_box->GetExtents());
		});
		ret &= AttachComponent(m_blocking_box);

		//무기 콜리전 설정 무기 Actor에서 해줄것
		m_weapon_collision_box->SetName("Weapon Collision");
		//m_weapon_collision_box->SetExtents(Vec3{}); //무기 박스 콜리전 크기
		//m_weapon_collision_box->OnCollisionResponse([this](const SPtr<SceneComponent>& component, const SPtr<Actor>& other_actor,
		//	const SPtr<SceneComponent>& other_component)
		//{
		//	const auto& player = std::dynamic_pointer_cast<RevivePlayer>(other_actor);
		//	if (player != nullptr) // 안해주면 가끔 터진다.
		//	{
		//		int player_hp = player->GetHP();
		//		if (player_hp > 0)
		//			player->Hit();
		//		LOG_INFO("충돌 부위 :" + other_component->GetName());
		//		component->SetCollisionInfo(false, false, "Player Hit", { "Player Hit" }, false); //충돌 처리후 꺼준다.
		//	}
		//});

		ret &= AttachComponent(m_weapon_collision_box);
		//공격 범위 구체
		m_attack_sphere->SetExtents(350.f);
		m_attack_sphere->OnCollisionResponse([this](const SPtr<SceneComponent>& component, const SPtr<Actor>& other_actor,
			const SPtr<SceneComponent>& other_component) {
			const auto& player = std::dynamic_pointer_cast<RevivePlayer>(other_actor);
			//m_player_position = other_actor->GetPosition();
			if (player != nullptr)
			{
				if (player->GetIsDying() == false)
				{
					Attack();
				}
			}
		});

		//Hit Box
		SPtr<BoxComponent> hit_box_1 = CreateSPtr<BoxComponent>(Vec3{60.f,60.f,60.f});
		hit_box_1->SetName("head hit box");
		hit_box_1->SetLocalPosition(Vec3{ 0.0f,330.f,0.f });
		m_hit_boxes.emplace_back(hit_box_1);
		SPtr<BoxComponent> hit_box_2 = CreateSPtr<BoxComponent>(Vec3{ 50.f,60.f,32.f });
		hit_box_2->SetName("body hit box");
		hit_box_2->SetLocalPosition(Vec3{ 0.0f,210.f,0.f });
		m_hit_boxes.emplace_back(hit_box_2);
		SPtr<BoxComponent> hit_box_3 = CreateSPtr<BoxComponent>(Vec3{ 20.f,65.f,30.f });
		hit_box_3->SetName("leg hit box");
		hit_box_3->SetLocalPosition(Vec3{ 40.0f,75.f,10.f });
		m_hit_boxes.emplace_back(hit_box_3);
		SPtr<BoxComponent> hit_box_4 = CreateSPtr<BoxComponent>(Vec3{ 20.f,65.f,30.f });
		hit_box_4->SetName("leg hit box");
		hit_box_4->SetLocalPosition(Vec3{ -40.0f,75.f,10.f });
		m_hit_boxes.emplace_back(hit_box_4);

		for (const auto& hit_box : m_hit_boxes)
		{
			hit_box->SetCollisionInfo(true, false, "enemy hit", { "bullet" }, false);
			ret &= AttachComponent(hit_box);
		}

		return ret;
	}
	void SkeletonKing::Attack()
	{
		if (m_is_attacking == false)
		{
			Enemy::Attack();
			//m_weapon_collision_box->SetCollisionInfo(true, false, "Player Hit", { "Player Hit" }, true);//공격 시 켜준다.
		}

	}
}