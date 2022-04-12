#include <include/client_core.h>
#include <client/object/component/mesh/skeletal_mesh_component.h>
#include <client/object/component/render/sphere_component.h>
#include <client/object/component/render/box_component.h>
#include <client/input/input.h>
#include "object/actor/Enemy.h"

namespace revive
{
	Enemy::Enemy(const std::string& mesh_path, const std::string& name)
		:Actor(eMobilityState::kMovable,name)
	{
		m_skeletal_mesh_component = CreateSPtr<SkeletalMeshComponent>();
		m_blocking_sphere = CreateSPtr<SphereComponent>(32.f, "Blocking Sphere");
		m_blocking_box = CreateSPtr<BoxComponent>(Vec3{ 32.f,32.f,32.f }, "Blocking Box");
		m_mesh_path = mesh_path;
	}
	bool Enemy::Initialize()
	{
		bool ret = true;

		ret &= m_skeletal_mesh_component->SetMesh(m_mesh_path);
		m_skeletal_mesh_component->SetAnimation("idle");
		m_skeletal_mesh_component->SetName(m_name + " Mesh");

		//Test
		if (Input::RegisterAxisEvent(m_name +" move forward", { AxisEventKeyInfo{eKey::kUArrow, 1.0f}, AxisEventKeyInfo{eKey::kDArrow, -1.0f} },
			[this](float axis)->bool { auto& curr_position = GetPosition(); SetPosition(curr_position + Vec3{ 0.0f,0.0f, axis*100.0f }); return true; }, true, eInputOwnerType::kActor))
			RegisterInputEvent(m_name + " move forward");

		if (Input::RegisterAxisEvent(m_name + " move right", { AxisEventKeyInfo{eKey::kRArrow, 1.0f}, AxisEventKeyInfo{eKey::kLArrow, -1.0f} },
			[this](float axis)->bool { auto& curr_position = GetPosition(); SetPosition(curr_position + Vec3{ axis * 100.0f,0.0f,0.0f  }); return true; }, true, eInputOwnerType::kActor))
			RegisterInputEvent(m_name + " move right");
		return ret;
	}
	void Enemy::Shutdown()
	{
		m_skeletal_mesh_component = nullptr;
		m_blocking_sphere = nullptr;
		for (auto& hit_box : m_hit_boxes)
			hit_box = nullptr;
	}
	void Enemy::Update(float delta_time)
	{
		if (m_disappear_time >= 1.0f)
			SetActorState(eActorState::kDead);
		if (m_is_disappearing)
			m_disappear_time += delta_time; 
		if (m_hp <= 0 && m_is_dead == false)
		{
			m_skeletal_mesh_component->SetAnimation("death", false);
			m_is_dead = true;
		}
		
	}
	void Enemy::FixYPosition()
	{
		Vec3 current_position = GetPosition();
		current_position.y = 300;
		SetPosition(current_position);
	}
	void Enemy::Hit(int damage)
	{
		m_skeletal_mesh_component->SetAnimation("hit", false);
		m_hp -= damage;
		LOG_INFO(m_name + " HP : {0}, 입은 피해 : {1}", m_hp, damage);
	}
}