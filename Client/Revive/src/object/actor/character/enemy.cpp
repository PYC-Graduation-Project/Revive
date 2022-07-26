#include <include/client_core.h>
#include <client/object/component/mesh/skeletal_mesh_component.h>
#include <client/object/component/render/sphere_component.h>
#include <client/object/component/render/box_component.h>
#include <client/input/input.h>
#include <client/physics/collision/collisioner/collisioner.h>
#include <client/object/component/util/simple_movement_component.h>
#include <client/object/component/util/character_movement_component.h>
#include<client/event/packetevent/packet_helper.h>
#include "object/actor/character/enemy.h"
#include "revive_server/message/message_event_info.h"

#include <client/object/component/render/widget_component.h>
#include "object/ui/enemy_info_ui_layer.h"
#include "object/actor/weapon/weapon.h"

namespace revive
{
	Enemy::Enemy(const std::string& mesh_path, const std::string& name)
		:Actor(eMobilityState::kMovable,name)
	{
		m_simple_movement_component = CreateSPtr<SimpleMovementComponent>();
		m_skeletal_mesh_component = CreateSPtr<SkeletalMeshComponent>();
		m_blocking_sphere = CreateSPtr<SphereComponent>(32.f, "Blocking Sphere");
		m_blocking_box = CreateSPtr<BoxComponent>(Vec3{ 32.f,32.f,32.f }, "Blocking Box");
		m_agro_sphere = CreateSPtr<SphereComponent>(1800.f, "agro sphere");
		m_hit_box = CreateSPtr<BoxComponent>(Vec3{ 32.f,32.f,32.f }, "hit box");
		m_mesh_path = mesh_path;

		m_ui_layer = CreateSPtr<EnemyInfoUILayer>();
		m_widget_component = CreateSPtr<WidgetComponent>(m_ui_layer);
	}

	bool Enemy::Initialize()
	{
		bool ret = true;

		ret &= m_skeletal_mesh_component->SetMesh(m_mesh_path);
		m_skeletal_mesh_component->SetAnimation("run");
		m_skeletal_mesh_component->SetName(m_name + " Mesh");

		m_simple_movement_component->SetMaxSpeed(145.0f);
		//m_character_movement_component->UseOrientRotationToMovement(true);
		ret &= AttachComponent(m_simple_movement_component);
		//ret &= AttachComponent(m_agro_sphere);
		m_agro_sphere->SetCollisionInfo(true, false, "enemy agro",{"player hit","base"}, true);
		

		//ret &= AttachComponent(m_attack_sphere);
		//m_attack_sphere->SetCollisionInfo(true, false, "enemy attack", { "player hit","base"}, true);
		
		//Test
		
		RegisterReceiveMessage(HashCode("move object"));

		m_ui_layer->SetEnemy(std::static_pointer_cast<Enemy>(shared_from_this()));
		m_widget_component->SetSize(Vec2(150.0f, 32.0f));
		ret &= AttachComponent(m_widget_component);

		return ret;
	}

	void Enemy::Shutdown()
	{
		m_widget_component = nullptr;
		m_ui_layer = nullptr;

		m_simple_movement_component = nullptr;
		m_skeletal_mesh_component = nullptr;
		m_blocking_sphere = nullptr;
		m_blocking_box = nullptr;
		m_agro_sphere = nullptr;
		m_hit_box = nullptr;
		m_weapon = nullptr;
	}

	void Enemy::Update(float delta_time)
	{

		m_time += delta_time;
		m_simple_movement_component->AddInputVector(m_velocity);
		if (m_time >= 0.5f)
		{
			m_time -= 0.5f;

			m_inter_velocity = m_previous_pos - GetPosition();
		}

		SetPosition(GetPosition() + m_inter_velocity * delta_time * 2.0f);

		if (m_disappear_time >= 0.1f)
		{
			m_is_disappearing = false;
			if(m_weapon != nullptr)
				m_weapon->SetActorState(eActorState::kDead);
			SetActorState(eActorState::kDead);
		}
		if (m_is_disappearing)
			m_disappear_time += delta_time; 

	}

	void Enemy::ExecuteMessageFromServer(const SPtr<MessageEventInfo>& message)
	{
			
		switch (message->GetEventID())
		{
		case HashCode("move object"): {
			auto msg = std::static_pointer_cast<MoveObjectMessageEventInfo>(message);

			if (GetPosition() != msg->GetObjPosition())
			{
				auto rot = FindLookAtRotation(GetPosition(), msg->GetObjPosition());
				SetRotation(rot);
			}
			
			//진짜 다음좌표 보내는것
			m_previous_pos = m_next_pos;
			m_velocity = msg->GetObjPosition() - m_previous_pos;
			m_next_pos = msg->GetObjPosition();
		
			break;
		}
		case HashCode("testmove"): {
			auto msg = std::static_pointer_cast<TestMessageEventInfo>(message);
			//msg->m_move_lock.lock();
			//m_rotating_component->SetRotatingRate(Vec3(0.0f, 180.0f, 0.0f));
			//SetPosition(Vec3(0.0f, 0.0f, 0.0f));
			//SetPosition(msg->GetObjPosition());

			//-----------방향벡터 보내는 테스트----------//
			//m_network_vec = msg->GetPosition();
			//------------------------------------------//


			//std::cout << msg->GetObjPosition() << std::endl;
			//Vec3 a{ msg->GetObjPosition() };
			//LOG_INFO(a);
			//msg->m_move_lock.unlock();
			break;
		}
		case HashCode("npc attack"):
		{
			auto msg = std::static_pointer_cast<NpcAttackEventInfo>(message);
			if (GetPosition() != msg->GetTargetPosition())
			{
				auto rot = FindLookAtRotation(GetPosition(), msg->GetTargetPosition());
				SetRotation(rot);
			}
			m_target_position = msg->GetTargetPosition();
			Attack();

			if (m_attack_packet_state_function != nullptr)
				m_attack_packet_state_function(GetNetworkID());

			break;
		}
		case HashCode("status change"):
		{
			auto msg = std::static_pointer_cast<StatusChangeEventInfo>(message);
			//LOG_INFO("나 맞았어 HP는 {0}이야", msg->GetObjHp());
			float hp = msg->GetObjHp();
			if (hp < 0) hp = 0;
			SetHP(hp);
			m_ui_layer->SetHPPercent(GetHP() / GetMaxHP());
			m_skeletal_mesh_component->SetAnimation("hit", false);
			m_is_attacking = false;

			break;
		}
		case HashCode("dead"):
		{
			PacketHelper::DisconnectActorFromServer(m_network_id);
			m_skeletal_mesh_component->SetAnimation("death", false);
			m_is_attacking = false;
			m_mob_count_decrement_function();
			break;
		}
		default:
			break;
		}
	}

	void Enemy::FixYPosition()
	{
		Vec3 current_position = GetPosition();
		current_position.y = 300;
		SetPosition(current_position);
	}

	Quaternion Enemy::FindLookAtRotation(const Vec3& start, const Vec3& target)
	{
		Vec3 direction = start - target;
		if(direction != vec3::ZERO )direction.Normalize();
		float angle = vec3::BetweenAngle(direction, vec3::AXIS_Z);
		if (vec3::Cross(direction, vec3::AXIS_Z, true).y > 0.0f) //0~2PI값을 얻기위한 if문
			angle = -angle;

		Quaternion rot = quat::CreateQuaternionFromRollPitchYaw(0.f, angle, 0.f);

		if (isnan(rot.x) || isnan(rot.y) || isnan(rot.z) || isnan(rot.w))
		{
			LOG_WARN("Rotation value is Nan {0}", rot);
			return GetRotation();
		}

		return rot;
	}

	void Enemy::Attack()
	{
		/*LOG_INFO(m_name + "이(가) 공격한다!");*/
		m_skeletal_mesh_component->SetAnimation("attack", false);
		m_is_attacking = true;
	}

	void Enemy::Hit(int damage, int nw_id) 
	{
		m_skeletal_mesh_component->SetAnimation("hit", false);
		//m_hp -= damage;
		PacketHelper::RegisterPacketEventToServer(CreateSPtr<ObjectHitMessageEventInfo>(HashCode("send hit"), m_network_id,nw_id));
		//LOG_INFO(m_name + " HP : {0}, 입은 피해 : {1}", m_hp, damage);
	}
	void Enemy::SetNetworkPosition(const Vec3& pos)
	{
		m_previous_pos = pos;
		m_next_pos = pos;
		SetPosition(pos);
	}
}