#include <include/client_core.h>
#include "object/actor/skeleton_king.h"
#include <client/object/component/mesh/skeletal_mesh_component.h>
#include <client/object/component/render/sphere_component.h>
#include <client/object/component/render/box_component.h>
#include "client/input/input.h"

namespace revive
{
	SkeletonKing::SkeletonKing(const std::string& name)
		:Enemy("Contents/TestKing/skeleton_king.rev", name)
	{
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

		ret &= SetCollisionComponent();
		
		if (Input::RegisterPressedEvent(m_name + " Test", { {eKey::kT} },
			[this]()->bool { m_skeletal_mesh_component->SetAnimation(m_animation_name[m_animation_select_num++]);
		if (m_animation_select_num >= m_animation_name.size()) m_animation_select_num = 0; return true; }
			, true, eInputOwnerType::kActor))
			RegisterInputEvent(m_name + " Test");
		
		m_hp = 20;
		SetPosition(Vec3{ 2400.0f,300.0f,4000.0f });
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
		m_blocking_sphere->SetCollisionInfo(false, false, "default", { "default" }, true);
		m_blocking_sphere->OnCollisionResponse([this](const SPtr<SceneComponent>& component, const SPtr<Actor>& other_actor,
			const SPtr<SceneComponent>& other_component) {
			FixYPosition();
			LOG_INFO(GetName() + ": Sphere component {0} Enemy Position {1} Extents {2}", m_blocking_sphere->GetWorldPosition(), this->GetPosition(), m_blocking_sphere->GetExtents());
		});

		ret &= AttachComponent(m_blocking_sphere);

		//멀티에서만 사용
		m_blocking_box->SetExtents(Vec3{50.f,200.f,50.f});
		m_blocking_box->SetLocalPosition(Vec3{ 0.0f,m_blocking_box->GetExtents().y,0.0f });
		m_blocking_box->SetCollisionInfo(true, false, "default", { "default" }, true);
		m_blocking_box->OnCollisionResponse([this](const SPtr<SceneComponent>& component, const SPtr<Actor>& other_actor,
			const SPtr<SceneComponent>& other_component) {
			//LOG_INFO(GetName() + ": Box component {0} Enemy Position {1} Extents {2}", m_blocking_box->GetWorldPosition(), this->GetPosition(), m_blocking_box->GetExtents());
		});
		ret &= AttachComponent(m_blocking_box);

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
			hit_box->SetCollisionInfo(true, false, "EnemyHit", { "EnemyHit" }, false);
			ret &= AttachComponent(hit_box);
		}

		return ret;
	}
}