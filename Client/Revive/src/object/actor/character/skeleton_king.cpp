#include <include/client_core.h>
#include <client/object/component/mesh/skeletal_mesh_component.h>
#include <client/object/component/render/sphere_component.h>
#include <client/object/component/render/box_component.h>
#include <client/input/input.h>
#include <client/physics/collision/collisioner/collisioner.h>
#include "object/actor/weapon/axe.h"
#include "object/actor/gameplaymechanics/base.h"
#include "object/actor/character/revive_player.h"
#include "object/actor/character/skeleton_king.h"
namespace revive
{
	SkeletonKing::SkeletonKing(const std::string& name)
		:Enemy("Contents/skeleton_king.rev", name)
	{
		m_weapon = CreateSPtr<Axe>();
		SpawnActor(m_weapon);
	}
	bool SkeletonKing::Initialize()
	{
		bool ret = true;

		ret &= Enemy::Initialize();
		ret &= AttachComponent(m_skeletal_mesh_component);
		m_skeletal_mesh_component->SetLocalRotation(math::ToRadian(-90.f), 0.f, 0.f);
		m_skeletal_mesh_component->AddNotify("death end", "death", 82,
			[this]() { m_is_disappearing = true;  });
		m_skeletal_mesh_component->AddNotify("hit end", "hit", 14,
			[this]() { m_skeletal_mesh_component->SetAnimation("run");  });
		m_skeletal_mesh_component->AddNotify("attack end", "attack", 50,
			[this]() { m_is_attacking = false; m_skeletal_mesh_component->SetAnimation("run");  m_weapon->SetIsCollision(false);/*공격 후에 재생할 애니메이션*/});
		
		m_weapon->SetAttachedActor(shared_from_this(),m_skeletal_mesh_component);
		m_weapon->SetSocketName("mount0");
		m_weapon->SetScale(0.6f);
		ret &= SetCollisionComponent();
		
		if (Input::RegisterPressedEvent(m_name + " Test", { {eKey::kT} },
			[this]()->bool { m_skeletal_mesh_component->SetAnimation(m_animation_name[m_animation_select_num++]);
		if (m_animation_select_num >= m_animation_name.size()) m_animation_select_num = 0; return true; }
			, true, eInputOwnerType::kActor))
			RegisterInputEvent(m_name + " Test");
		
		m_hp = 20;
		//mesh_rotate = Vec3{ -90.f,0.f,0.f };
		SetScale(0.6f);

		return ret;
	}

	void SkeletonKing::Update(float delta_time)
	{
		Enemy::Update(delta_time);


	}

	void SkeletonKing::Shutdown()
	{
		Enemy::Shutdown();
		m_weapon->SetActorState(eActorState::kDead);
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
			//LOG_INFO(GetName() + ": Sphere component {0} Enemy Position {1} Extents {2}", m_blocking_sphere->GetWorldPosition(), this->GetPosition(), m_blocking_sphere->GetExtents());
		});

		//ret &= AttachComponent(m_blocking_sphere);

		//멀티에서만 사용
		m_blocking_box->SetExtents(Vec3{50.f,200.f,50.f});
		m_blocking_box->SetLocalPosition(Vec3{ 0.0f,m_blocking_box->GetExtents().y,0.0f });
		m_blocking_box->SetCollisionInfo(false, false, "enemy", { "wall" }, true);
		m_blocking_box->OnCollisionResponse([this](const SPtr<SceneComponent>& component, const SPtr<Actor>& other_actor,
			const SPtr<SceneComponent>& other_component) {
			//LOG_INFO(GetName() + ": Box component {0} Enemy Position {1} Extents {2}", m_blocking_box->GetWorldPosition(), this->GetPosition(), m_blocking_box->GetExtents());
		});
		//ret &= AttachComponent(m_blocking_box);

		//공격 범위 구체
		m_agro_sphere->OnCollisionResponse([this](const SPtr<SceneComponent>& component, const SPtr<Actor>& other_actor,
			const SPtr<SceneComponent>& other_component) {
			float distance = (GetPosition() - other_actor->GetPosition()).Length();
			
			if (distance < 350.f)
			{
				const auto& player = std::dynamic_pointer_cast<DefaultPlayer>(other_actor);
				if (player != nullptr)
				{
					if (player->GetIsDying() == false)
					{
						SetRotation(FindLookAtRotation(GetPosition(), other_actor->GetPosition()));
						Attack();
					}
				}
				else
				{
					const auto& base = std::dynamic_pointer_cast<Base>(other_actor);
					if (base != nullptr)
					{
						int base_hp = base->GetHP();
						LOG_INFO(base_hp);
						if (base_hp > 0)
						{
							SetRotation(FindLookAtRotation(GetPosition(), other_actor->GetPosition()));

							Attack();
						}
					}
				}
			}
			//LOG_INFO("시야 범위에 인식된 플레이어 :" + other_actor->GetName());
		});

		//Hit Box
		m_hit_box->SetExtents(Vec3{ 60.f,200.f,60.f });
		m_hit_box->SetLocalPosition(Vec3{ 0.0f,200.f,0.f });
		m_hit_box->SetCollisionInfo(true, false, "enemy hit", { "bullet" }, false);
		ret &= AttachComponent(m_hit_box);

		return ret;
	}

	void SkeletonKing::Attack()
	{
		if (m_is_attacking == false)
		{
			m_weapon->SetCollisionInfo(true, false, true);
			Enemy::Attack();
		}

	}
}