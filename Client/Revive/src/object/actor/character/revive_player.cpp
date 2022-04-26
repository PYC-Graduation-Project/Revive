#include <include/client_core.h>
#include <client/object/component/core/scene_component.h>
#include <client/object/component/util/simple_movement_component.h>
#include <client/object/component/mesh/static_mesh_component.h>
#include <client/object/component/mesh/skeletal_mesh_component.h>
#include <client/object/component/render/sphere_component.h>
#include <client/object/component/render/box_component.h>
#include <client/object/component/util/render_camera_component.h>
#include <client/object/component/util/character_movement_component.h>
#include <client/object/component/util/simple_movement_component.h>
#include <client/object/actor/player_controller.h>
#include <client/object/actor/core/actor.h>
#include <client/input/input.h>
#include<client/event/packetevent/packet_helper.h>
#include "object/actor/weapon/pistol.h"
#include "object/actor/character/enemy.h"
#include "object/actor/character/revive_player.h"
#include "object/actor/projectile/projectile.h"
#include "object/actor/projectile/bullet.h"
#include "object/statemachine/state_machine.h"
#include "revive_server/message/message_event_info.h"

namespace revive
{
	DefaultPlayer::DefaultPlayer(const std::string& name)
		:Pawn(name)
	{
		m_character_movement_component = CreateSPtr<CharacterMovementComponent>();
		m_skeletal_mesh_component = CreateSPtr<SkeletalMeshComponent>();
		m_hit_box = CreateSPtr<BoxComponent>();
		m_player_fsm = CreateSPtr<PlayerFSM>();
		m_mesh_path = "Contents/violet.rev";
		for (auto& weapon : m_weapon)
		{
			weapon = CreateSPtr<Pistol>();
			SpawnActor(weapon);
			weapon->SetScale(0.2f);
		}
	}

	bool DefaultPlayer::Initialize()
	{
		bool ret = m_skeletal_mesh_component->SetMesh(m_mesh_path);
		ret &= AttachComponent(m_skeletal_mesh_component);
		m_skeletal_mesh_component->SetLocalRotation(math::ToRadian(-90.0f), math::ToRadian(180.0f), 0.0f);
		m_skeletal_mesh_component->SetLocalScale(0.01f);

		//공격2 전환 및 종료
		m_skeletal_mesh_component->AddNotify("First Attack", "attack_left", 17,
			[this]() {
			SetAnimation("attack_right",false); 
			SetAnimationSpeed(1.0f);
			m_is_fire = false;
			 });
		m_skeletal_mesh_component->AddNotify("Second Attack End", "attack_right", 17,
			[this]() {m_is_attacking = false; m_is_fire = false; LOG_INFO(m_is_attacking); });

		//총알 발사(스폰) 타이밍
		m_skeletal_mesh_component->AddNotify("First Fire", "attack_left", 9,
			[this]() {	Attack(); LOG_INFO(m_is_attacking); });
		m_skeletal_mesh_component->AddNotify("Second Fire", "attack_right", 10,
			[this]() {  Attack(); LOG_INFO(m_is_attacking); });

		m_skeletal_mesh_component->AddNotify("Hit End", "hit", 8,
			[this]() { m_is_hitting = false; /*LOG_INFO(m_is_attacking);*/ });

		ret &= AttachComponent(m_character_movement_component);
		m_character_movement_component->SetMaxSpeed(100.f);
		m_character_movement_component->UseOrientRotationToMovement(true);

		std::array<std::string, 2> weapon_names = { "left", "right" };
		std::array<std::string, 2> socket_names = { "Bip001_L_Hand", "Bip001_R_Hand" };
		std::array<Vec3, 2> pos_offset = { Vec3{ -80.f, 10.f,0.f }, Vec3{-80.f,20.f,0.f} };
		std::array<Vec3, 2> rot_offset = { Vec3{ 90.f, 90.f,-90.f }, Vec3{90.f,90.f,-55.f} };

		for (int i = 0; i < 2; ++i)
		{
			m_weapon[i]->SetName(m_weapon[i]->GetName() + weapon_names[i]);
			m_weapon[i]->SetAttachedActor(shared_from_this(), m_skeletal_mesh_component);
			m_weapon[i]->SetSocketName(socket_names[i]);
			m_weapon[i]->SetPositionOffset(pos_offset[i]);
			m_weapon[i]->SetRotationOffset(rot_offset[i]);
		}

		m_player_fsm->Initialize(SharedFromThis());

		//Hit Box
		m_hit_box->SetExtents(Vec3{ 40.f,80.f,40.f });
		m_hit_box->SetLocalPosition(Vec3{ 0.0f,80.f,0.f });
		m_hit_box->SetName("hit box");
		m_hit_box->SetCollisionInfo(true, false, "player hit", { "stone","axe", "enemy agro"}, false);
		ret &= AttachComponent(m_hit_box);

		SetScale(0.5f);

		return ret;
	}

	void DefaultPlayer::Shutdown()
	{
		m_character_movement_component = nullptr;
		m_skeletal_mesh_component = nullptr;
		m_hit_box = nullptr;
		m_player_fsm->Shutdown();
		m_player_fsm = nullptr;
		for (auto& weapon : m_weapon)
			weapon = nullptr;
	}

	void DefaultPlayer::Update(float delta_time)
	{
		m_player_fsm->Update();

		PlayerInterpolation(delta_time);

		if (m_speed > 0)
		{
			m_speed -= 4000 * delta_time;
		}
		m_speed = std::clamp(m_speed, 0.f, 300.f);
	}

	void DefaultPlayer::PlayerInterpolation(float delta_time)
	{
		m_time += delta_time;

		if (m_previous_velocity == m_velocity)
			m_stop_time += delta_time;
		else
			m_stop_time = 0.f;

		if (m_stop_time >= 0.1f)
		{
			m_stop_time -= 0.1f;
			m_velocity = vec3::ZERO;
			m_previous_velocity = vec3::ZERO;
		}
		m_character_movement_component->AddInputVector(m_velocity);

		if (m_time >= 0.5f)
		{
			m_time -= 0.5f;

			m_inter_velocity = m_previous_pos - GetPosition();
		}

		if (m_velocity != vec3::ZERO)
			SetPosition(GetPosition() + m_inter_velocity * delta_time * 2.0f);
	}

	void DefaultPlayer::ExecuteMessageFromServer(const SPtr<MessageEventInfo>& message)
	{
		switch (message->GetEventID())
		{
			case HashCode("move object"): 
			{
				auto msg = std::static_pointer_cast<MoveObjectMessageEventInfo>(message);
				SetRotation(msg->GetObjRotation());

				m_previous_pos = m_next_pos;
				m_previous_velocity = m_velocity;
				m_velocity = msg->GetObjPosition() - m_previous_pos;
				m_next_pos = msg->GetObjPosition();
				m_speed += m_velocity.Length();
				break;
			}
			case HashCode("player attack"):
			{
				LOG_INFO("공격 패킷 받음");
				auto msg = std::static_pointer_cast<RecvAttackEventInfo>(message);
				m_is_attacking = true;
				break;
			}
			case HashCode("dead"):
			{
				m_is_dying = true;
			}
		}
	}

	Quaternion DefaultPlayer::FindLookAtRotation(const Vec3& start, const Vec3& target)
	{
		//Vec3 old_player_forward = GetForward();

		//if (vec3::Dot(old_player_forward, dest_direction) == -1)//현재보는 방향과 반대방향으로 회전 시 너무느리지않게 회전시키려면 필요한 부분
		//	old_player_forward += Vec3{ 0.02f,0.0f,-0.02f };

		//float rotate_speed = 70.0f * 0.016f; //회전속도 delta_time을곱해야하나 없으니까 0.016곱함

		//Vec3 curr_player_forward = /*old_player_forward +*/ dest_direction;//* rotate_speed;
		//curr_player_forward = vec3::Normalize(curr_player_forward);

		//float angle = vec3::BetweenAngle(curr_player_forward, vec3::AXIS_Z); //0~PI만 반환함 (radian)

		//if (vec3::Cross(curr_player_forward, vec3::AXIS_Z, true).y > 0.0f) //0~2PI값을 얻기위한 if문
		//	angle = -angle;

		//auto player_rot = quat::CreateQuaternionFromRollPitchYaw(0.0f, angle, 0.0f); //angle만큼 Y축 회전
		//SetRotation(player_rot);

		Vec3 direction = start - target;
		direction.Normalize();
		float angle = vec3::BetweenAngle(direction, vec3::AXIS_Z);
		if (vec3::Cross(direction, vec3::AXIS_Z, true).y > 0.0f) //0~2PI값을 얻기위한 if문
			angle = -angle;
		return quat::CreateQuaternionFromRollPitchYaw(0.f, angle, 0.f);
		/*Vec3 rotate_player = Vec3{ 0.f,math::ToDegrees(angle),0.f };
		return quat::CreateQuaternionFromRollPitchYaw(math::ToRadian(rotate_player.x), math::ToRadian(rotate_player.y), math::ToRadian(rotate_player.z));*/

	}

	const float DefaultPlayer::GetVelocity() const
	{
		return m_speed;
		//return m_character_movement_component->GetVelocity().Length();
	}

	void DefaultPlayer::SetAnimation(const std::string& animation_name, bool looping)
	{
		m_skeletal_mesh_component->SetAnimation(animation_name, looping);
	}

	void DefaultPlayer::SetMeshPosition(const Vec3& pos)
	{
		m_skeletal_mesh_component->SetLocalPosition(pos);
	}

	void DefaultPlayer::SetAnimationSpeed(float speed)
	{
		m_skeletal_mesh_component->SetAnimationSpeed(speed);
	}

	void DefaultPlayer::Attack()
	{
		if (m_is_fire == false)
		{
			//카메라가 보는 방향으로 총을 쏴야한다(회전)
			Vec3 direction = GetForward();
			direction.y = 0;
			SetRotation(FindLookAtRotation(direction, vec3::ZERO));

			//총알 스폰
			const auto& bullet = CreateSPtr<Bullet>();
			bullet->SetPosition(GetPosition() + Vec3{ 0.0f,50.0f,0.0f });
			bullet->SetBlockingSphereRadius(10.f);
			bullet->SetVelocity(GetForward());

			// 다른 플레이어는 충돌처리가 필요가없다.
			//bullet->SetCollisionInfo(true, "bullet", { "enemy hit" }, true);
			//bullet->SetOnCollisionResponse([bullet,this](const SPtr<SceneComponent>& component, const SPtr<Actor>& other_actor,
			//	const SPtr<SceneComponent>& other_component)
			//{
			//	bullet->SetCollisionInfo(false, false, false);
			//	LOG_INFO(component->GetName() + " " + other_actor->GetName() + " " + other_component->GetName());
			//	//const auto& enemy = std::dynamic_pointer_cast<Enemy>(other_actor);
			//	//if (enemy != nullptr)
			//	//{
			//	//		int enemy_hp = enemy->GetHP();
			//	//		if (enemy_hp > 0)
			//	//			//enemy->Hit(0,m_network_id);
			//	//}
			//	LOG_INFO("충돌 부위 :" + other_component->GetName());
			//	bullet->SetActorState(eActorState::kDead);
			//});

			SpawnActor(bullet);
			m_is_fire = true;
		}
		
	}


	void DefaultPlayer::Hit(int damage, int nw_id)
	{
		++m_hit_count;
		m_is_hitting = true;
		//m_hp -= damage;
	}

	RevivePlayer::RevivePlayer(const std::string& name)
		: DefaultPlayer(name)
	{
		m_camera_component = CreateSPtr<RenderCameraComponent>("Follow Camera");
		m_blocking_sphere = CreateSPtr<SphereComponent>(40.0f,"Player Blocking Collision");
		m_movement_component = CreateSPtr<CharacterMovementComponent>();
	}

	bool RevivePlayer::Initialize()
	{
		bool ret = DefaultPlayer::Initialize();

		ret &= AttachComponent(m_movement_component);
		m_movement_component->SetMaxSpeed(300.f);// 타일 크기 300 * 0.75(tile/s)  = 225 (근데 너무느리다)
		m_movement_component->UseOrientRotationToMovement(true);

		ret &= AttachComponent(m_blocking_sphere);
		m_blocking_sphere->SetLocalPosition(Vec3{ 0.0f,m_blocking_sphere->GetExtents().y,0.0f });
		m_blocking_sphere->SetCollisionInfo(true, true, "player", { "base","wall"}, true);
		m_blocking_sphere->OnCollisionResponse([this](const SPtr<SceneComponent>& component, const SPtr<Actor>& other_actor,
				const SPtr<SceneComponent>& other_component) {
			FixYPosition();
			//LOG_INFO("Player sphere component {0} Player Position {1} Extents {2}", m_blocking_sphere->GetWorldPosition(), this->GetPosition(), m_blocking_sphere->GetExtents());
			});

		const auto& player_controller = std::dynamic_pointer_cast<PlayerController>(m_controller.lock());
		if (player_controller != nullptr)
			player_controller->SetPlayerCamera(m_camera_component);
		ret &= AttachComponent(m_camera_component);
		m_camera_component->UseControllerRotation(true);
		m_camera_component->SetLocalPosition(Vec3(0.0f, 200.0f, -500.0f));

		RegisterEvent();
		
		SetPosition(Vec3{ 2400.0f,300.0f,3400.0f });
		

		SetUseControllerPitch(false);
		SetUseControllerYaw(false);
		SetUseControllerRoll(false);

		return ret;
	}  

	void RevivePlayer::Update(float delta_time)
	{
		//DefaultPlayer::Update(delta_time);
		m_player_fsm->Update();
	}

	void RevivePlayer::ExecuteMessageFromServer(const SPtr<MessageEventInfo>& message)
	{
		switch (message->GetEventID())
		{
		case HashCode("dead"):
		{
			m_is_dying = true;
		}
		}
	}

	void RevivePlayer::Shutdown()
	{
		DefaultPlayer::Shutdown();
		m_movement_component = nullptr;
		m_blocking_sphere = nullptr;
		m_camera_component = nullptr;
	}

	void RevivePlayer::RegisterEvent()
	{
		//테스트용 명령키
		//맞는 도중 다시 또 맞을 수 있다.
		RegisterPressedEvent("Hit", { {eKey::kP} },
			[this]()->bool { ++m_hit_count; m_is_hitting = true; DecrementHP();  return true; });
		RegisterPressedEvent("Cheat Mode", { {eKey::kC} },
			[this]()->bool {  m_is_cheating = !m_is_cheating; 
		if (m_is_cheating)LOG_INFO("Cheat Mode On"); 
		else LOG_INFO("Cheat Mode Off");
		return true; });

		//공격
		RegisterPressedEvent("attack", { {eKey::kLButton} },
			[this]()->bool {  
			bool ret = false;
			if(m_is_dying == false)
				if (m_is_attacking == false) {
					ret = m_is_attacking = true;
					PacketHelper::RegisterPacketEventToServer(CreateSPtr<SendAttackEventInfo>(HashCode("send attack"),Vec3(0.f,0.f,0.f), Vec3(0.f, 0.f, 0.f)));
				//공격추가
				}
			return ret;
		});
		
	}

	const float RevivePlayer::GetVelocity() const
	{
		return m_movement_component->GetVelocity().Length();
	}

	void RevivePlayer::AddMovementInput(const Vec3& direction, float scale)
	{
		m_movement_component->AddInputVector(Vec3{ direction.x,0.0f,direction.z } * scale);
	}

	void RevivePlayer::Attack()
	{
		if (m_is_fire == false)
		{
			Vec3 direction = m_controller.lock()->GetForward();
			direction.y = 0;
			SetRotation(FindLookAtRotation(direction, vec3::ZERO));

			//총알 스폰
			const auto& bullet = CreateSPtr<Bullet>();
			bullet->SetPosition(GetPosition() + Vec3{ 0.0f,50.0f,0.0f });
			bullet->SetBlockingSphereRadius(10.f);
			bullet->SetVelocity(direction);//컨트롤러의 방향으로 총알을 발사한다.
			LOG_INFO("공격 패킷 보냄");	
			bullet->SetCollisionInfo(true, "bullet", { "enemy hit" }, true);

			bullet->SetOnCollisionResponse([bullet,this](const SPtr<SceneComponent>& component, const SPtr<Actor>& other_actor,

				const SPtr<SceneComponent>& other_component)
			{
				bullet->SetCollisionInfo(false, false, false);

				LOG_INFO(component->GetName() + " " + other_actor->GetName() + " " + other_component->GetName());
				const auto& enemy = std::dynamic_pointer_cast<Enemy>(other_actor);
				if (enemy != nullptr)
				{
						int enemy_hp = enemy->GetHP();
						if (enemy_hp > 0)

							enemy->Hit(0,m_network_id);


							LOG_INFO("충돌 부위 :" + other_component->GetName());
						bullet->SetActorState(eActorState::kDead);
			

				}
			});
			SpawnActor(bullet);
			m_is_fire = true;
		}
	}


	void RevivePlayer::Hit(int damage, int nw_id)

	{
		++m_hit_count;
		m_is_hitting = true;
		if (m_is_cheating == false)m_hp -= damage;
		else --m_hit_count;

		LOG_INFO(nw_id);

		PacketHelper::RegisterPacketEventToServer(CreateSPtr<ObjectHitMessageEventInfo>(HashCode("send hit"), m_network_id,nw_id));
	}

	//void RevivePlayer::MinPitch()
	//{
	//	Vec3 rot = quat::QuaternionToEuler(m_controller.lock()->GetRotation());
	//	if (math::ToDegrees(rot.x) < 5) //컨트롤러에서 SetRotation(rot);를 안해주는게 제일 좋은 방법
	//	{
	//		rot.x += math::ToRadian(5.0f) - rot.x;
	//		m_controller.lock()->SetRotation(quat::CreateQuaternionFromRollPitchYaw(rot.x, rot.y, rot.z));
	//	}
	//}

	//void RevivePlayer::RotateFromDirection(const Vec3& direction)
	//{

	//	Vec3 curr_player_forward = /*old_player_forward +*/ direction;//* rotate_speed;
	//	curr_player_forward = vec3::Normalize(curr_player_forward);

	//	float angle = vec3::BetweenAngle(curr_player_forward, vec3::AXIS_Z); //0~PI만 반환함 (radian)

	//	if (vec3::Cross(curr_player_forward, vec3::AXIS_Z, true).y > 0.0f) //0~2PI값을 얻기위한 if문
	//		angle = -angle;
	//	LOG_INFO(math::ToDegrees(angle));
	//	auto player_rot = quat::CreateQuaternionFromRollPitchYaw(0.0f, angle, 0.0f); //angle만큼 Y축 회전
	//	SetRotation(player_rot);
	//}

	void RevivePlayer::FixYPosition()
	{
		Vec3 current_position = GetPosition();
		current_position.y = 300.f;
		SetPosition(current_position);
	}

	DefaultCharacter::DefaultCharacter(const std::string& name)
		:DefaultPawn(name)
	{
	}

	bool DefaultCharacter::Initialize()
	{
		bool ret = true;

		ret &= Pawn::Initialize();
		ret &= AttachComponent(m_movement_component);

		auto mesh = CreateSPtr<StaticMeshComponent>();
		ret &= mesh->SetMesh("../Contents/cube.obj");
		mesh->SetCollisionInfo(true, false, "default", { "default" },true);
		mesh->OnCollisionResponse([this](const SPtr<SceneComponent>& component, const SPtr<Actor>& other_actor,
			const SPtr<SceneComponent>& other_component) {
			CollisionResponse(component, other_actor, other_component);
		});
		ret &= AttachComponent(mesh);

		RegisterAxisEvent("move forward", { AxisEventKeyInfo{eKey::kW, 1.0f}, AxisEventKeyInfo{eKey::kS, -1.0f} },
			[this](float axis)->bool { AddMovementInput(GetForward(), axis); return true; });
		RegisterAxisEvent("move right", { AxisEventKeyInfo{eKey::kD, 1.0f}, AxisEventKeyInfo{eKey::kA, -1.0f} },
			[this](float axis)->bool { AddMovementInput(GetRight(), axis); return true; });
		RegisterAxisEvent("move up", { AxisEventKeyInfo{eKey::kE, 1.0f}, AxisEventKeyInfo{eKey::kQ, -1.0f} },
			[this](float axis)->bool { AddMovementInput(GetUp(), axis); return true; });
		RegisterAxisEvent("turn", { AxisEventKeyInfo{eKey::kXMove, 1.0f} },
			[this](float axis)->bool {
			IVec2 relative_pos = Input::GetRelativeMousePosition();
			return AddControllerYawInput(axis * relative_pos.x);
		});
		RegisterAxisEvent("look up", { AxisEventKeyInfo{eKey::kYMove, 1.0f} },
			[this](float axis)->bool {
			IVec2 relative_pos = Input::GetRelativeMousePosition();
			return AddControllerPitchInput(axis * relative_pos.y);
		});

		SetUseControllerPitch(true);
		SetUseControllerYaw(true);
		SetUseControllerRoll(true);
		return ret;
	}
	void DefaultCharacter::Shutdown()
	{
		m_movement_component = nullptr;
		Pawn::Shutdown();
	}
	void DefaultCharacter::Update(float delta_time)
	{
		Pawn::Update(delta_time);
	}
	void DefaultCharacter::CollisionResponse(const SPtr<SceneComponent>& component, const SPtr<Actor>& other_actor, const SPtr<SceneComponent>& other_component)
	{
		LOG_INFO("{0}가 {1}에 충돌했다", component->GetOwner().lock()->GetName(), other_component->GetName());
	}
	
	

}

