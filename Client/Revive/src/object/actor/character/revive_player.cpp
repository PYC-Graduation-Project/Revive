#include <include/client_core.h>
#include <client/object/level/core/level_manager.h>
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

#include <client/object/component/light/spot_light_component.h>

#include "object/level/sharedinfo/revive_level_shared_info.h"
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

		m_spot_light_component = CreateSPtr<SpotLightComponent>();
	}

	bool DefaultPlayer::Initialize()
	{
		bool ret = m_skeletal_mesh_component->SetMesh(m_mesh_path);
		ret &= AttachComponent(m_skeletal_mesh_component);
		m_skeletal_mesh_component->SetLocalRotation(math::ToRadian(-90.0f), math::ToRadian(180.0f), 0.0f);
		m_skeletal_mesh_component->SetLocalScale(100.f);
		
		//죽었으니 장비 벗어
		m_skeletal_mesh_component->AddNotify("unequip", "death", 100, [this]() { 
			Unequip(); 
			m_skeletal_mesh_component->SetIsPlaying(false);
		});

		//공격2 전환 및 종료
		m_skeletal_mesh_component->AddNotify("First Attack", "attack_first", 17,
			[this]() {
			SetAnimation("attack_second",false); 
			m_is_fire = false;
			 });
		m_skeletal_mesh_component->AddNotify("Second Attack End", "attack_second", 17,
			[this]() {m_is_attacking = false; m_is_fire = false;  });

		//총알 발사(스폰) 타이밍
		m_skeletal_mesh_component->AddNotify("First Fire", "attack_first", 9,
			[this]() {	Attack(); /*LOG_INFO(m_is_attacking);*/ });
		m_skeletal_mesh_component->AddNotify("Second Fire", "attack_second", 10,
			[this]() {  Attack(); /*LOG_INFO(m_is_attacking);*/ });

		m_skeletal_mesh_component->AddNotify("Hit End", "hit", 8,
			[this]() { m_is_hitting = false; /*LOG_INFO(m_is_attacking);*/ });

		ret &= AttachComponent(m_character_movement_component);
		m_character_movement_component->SetMaxSpeed(100.f);
		m_character_movement_component->UseOrientRotationToMovement(true);

		Equip();

		m_player_fsm->Initialize(SharedFromThis());

		//Hit Box
		m_hit_box->SetExtents(Vec3{ 40.f,80.f,40.f });
		m_hit_box->SetLocalPosition(Vec3{ 0.0f,80.f,0.f });
		m_hit_box->SetName("hit box");
		m_hit_box->SetCollisionInfo(true, false, "player hit", { "stone","axe", "enemy agro"}, false);
		ret &= AttachComponent(m_hit_box);

		SetScale(0.5f);

		{
			const auto& game_option = std::static_pointer_cast<ReviveLevelSharedInfo>(LevelManager::GetLevelManager().GetLevelSharedInfo())->GetGameOption();

			m_spot_light_component->SetLightColor(Vec3(100000.0f, 100000.0f, 100000.0f));
			m_spot_light_component->SetAttenuationRadius(2048.0f);
			if (game_option->shadow_enable)
			{
				switch (game_option->shadow_quality)
				{
				case eShadowQuality::kLow: 	m_spot_light_component->SetShadowTextureSize(512); break;
				case eShadowQuality::kMiddle: m_spot_light_component->SetShadowTextureSize(1024); break;
				case eShadowQuality::kHigh: m_spot_light_component->SetShadowTextureSize(2048); break;
				case eShadowQuality::kVeryHigh: m_spot_light_component->SetShadowTextureSize(4096); break;
				default: break;
				}
			}
			else
			{
				m_spot_light_component->DisableShadow();
			}
			m_spot_light_component->SetConeOuterAngle(30.0f);
			m_spot_light_component->SetLocalPosition(Vec3(0.0f, 180.0f, 60.0f));
			ret &= AttachComponent(m_spot_light_component);
		}
		
		return ret;
	}
	
	void DefaultPlayer::Shutdown()
	{
		m_character_movement_component = nullptr;
		m_skeletal_mesh_component = nullptr;
		m_hit_box = nullptr;
		m_player_fsm->Shutdown();
		m_player_fsm = nullptr;
		if (IsEquipped() == true) Unequip();
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
		//LOG_INFO(" {0}  {1}", m_network_id, GetPosition());
		m_previous_velocity = m_velocity;

		if (m_is_attacking)
			//LOG_INFO("받음 {0}", m_attack_direction);
			SetRotation(FindLookAtRotation(m_attack_direction, vec3::ZERO));

	}

	void DefaultPlayer::PlayerInterpolation(float delta_time)
	{
		m_time += delta_time;

		if (floor(m_previous_velocity.Length() - m_velocity.Length()) == 0)
			m_stop_time += delta_time;

		if (m_stop_time >= 0.5f)
		{
			m_stop_time -= 0.5f;
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

				//SetRotation(msg->GetObjRotation());
				auto pos = msg->GetObjPosition();
				if (pos.x > 0 && pos.y > 0 && pos.z > 0)
				{
					m_previous_pos = m_next_pos;
					m_velocity = pos - m_previous_pos;
					m_next_pos = pos;
					m_speed += m_velocity.Length();
					m_stop_time = 0.f;
				}
				else
				{
					LOG_WARN(pos);
				}
				
				break;
			}
			case HashCode("player attack"):
			{
				//LOG_INFO("공격 패킷 받음");
				auto msg = std::static_pointer_cast<RecvAttackEventInfo>(message);
				m_attack_direction = msg->GetForward();
				m_is_attacking = true;
				break;
			}
			case HashCode("dead"):
			{
				PacketHelper::DisconnectActorFromServer(m_network_id);
				m_is_dying = true;
				break;
			}
			case HashCode("status change"):
			{
				auto msg = std::static_pointer_cast<StatusChangeEventInfo>(message);
				//LOG_INFO("나 맞았어 HP는 {0}이야", msg->GetObjHp());
				SetHP(msg->GetObjHp());
				break;
			}
		}
	}

	Quaternion DefaultPlayer::FindLookAtRotation(const Vec3& start, const Vec3& target)
	{
		Vec3 direction = start - target;
		if(direction != vec3::ZERO) direction.Normalize();
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

	const float DefaultPlayer::GetVelocity() const
	{
		return m_character_movement_component->GetVelocity().Length();
	}

	void DefaultPlayer::GetAnimationSpeed() const
	{
		m_skeletal_mesh_component->GetAnimationSpeed();
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

	void DefaultPlayer::SetHP(float hp)
	{
		m_hp = hp;
		if (m_changed_hp_function != nullptr)
			m_changed_hp_function(m_hp, m_max_hp);
	}

	void DefaultPlayer::SetNetworkPosition(const Vec3& pos)
	{
		m_previous_pos = pos;
		m_next_pos = pos;
		SetPosition(pos);
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


	void DefaultPlayer::Hit(float damage, int nw_id)
	{
		++m_hit_count;
		m_is_hitting = true;
		//m_hp -= damage;
	}

	void DefaultPlayer::Equip()
	{
		m_is_equipped = true;
		std::array<std::string, 2> weapon_names = { "left", "right" };
		std::array<std::string, 2> socket_names = { "Bip001_L_Hand", "Bip001_R_Hand" };
		std::array<Vec3, 2> pos_offset = { Vec3{ -80.f, 10.f,0.f }, Vec3{ -80.f,20.f,0.f } };
		std::array<Vec3, 2> rot_offset = { Vec3{ 90.f, 90.f,-90.f }, Vec3{ -40.f,190.f,0.f } };

		for (int i = 0; i < 2; ++i)
		{
			m_weapon[i]->SetName(m_weapon[i]->GetName() + weapon_names[i]);
			m_weapon[i]->SetAttachedActor(shared_from_this(), m_skeletal_mesh_component);
			m_weapon[i]->SetSocketName(socket_names[i]);
			m_weapon[i]->SetPositionOffset(pos_offset[i]);
			m_weapon[i]->SetRotationOffset(rot_offset[i]);
		}
	}

	void DefaultPlayer::Unequip()
	{
		m_is_equipped = false;
		for (const auto& weapon : m_weapon)
			weapon->SetActorState(eActorState::kDead);
	}

	RevivePlayer::RevivePlayer(const std::string& name)
		: DefaultPlayer(name)
	{
		m_camera_component = CreateSPtr<SpringArmRenderCameraComponent>("Follow Camera");
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
		m_blocking_sphere->SetCollisionInfo(true, true, "player", { "base","wall","healer"}, true);
		m_blocking_sphere->OnCollisionResponse([this](const SPtr<SceneComponent>& component, const SPtr<Actor>& other_actor,
				const SPtr<SceneComponent>& other_component) {
			FixYPosition();
			//LOG_INFO("Player sphere component {0} Player Position {1} Extents {2}", m_blocking_sphere->GetWorldPosition(), this->GetPosition(), m_blocking_sphere->GetExtents());
			});

		const auto& player_controller = std::dynamic_pointer_cast<PlayerController>(m_controller.lock());
		if (player_controller != nullptr)
			player_controller->SetPlayerCamera(m_camera_component);

		m_camera_component->UseControllerRotation(true);
		m_camera_component->SetMaxDistance(350.0f);
		m_camera_component->SetSpringSpeed(800.0f);
		m_camera_component->SetLocalPosition(Vec3(0.0f, 200.0f, 0.0f));
		m_camera_component->SetSpringArmTargetPosition(Vec3(0.0f, 200.0f, 0.0f));
		m_camera_component->SetCollisionInfo(true, false, "player camera", { "ground","base", "wall"}, true);
		ret &= AttachComponent(m_camera_component);

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
		if (m_is_cheating)
			m_is_attacking = true;

		if (m_is_attacking)
		{
			Vec3 direction = m_controller.lock()->GetForward();
			direction.y = 0;
			SetRotation(FindLookAtRotation(direction, vec3::ZERO));
		}

	}

	void RevivePlayer::ExecuteMessageFromServer(const SPtr<MessageEventInfo>& message)
	{
		switch (message->GetEventID())
		{
		case HashCode("dead"):
		{
			PacketHelper::DisconnectActorFromServer(m_network_id);
			break;
		}
		case HashCode("status change"):
		{
			auto msg = std::static_pointer_cast<StatusChangeEventInfo>(message);
			//LOG_INFO("나 맞았어 HP는 {0}이야", msg->GetObjHp());
			SetHP(msg->GetObjHp());

			break;
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
					Vec3 direction = m_controller.lock()->GetForward();
					direction.y = 0;
					PacketHelper::RegisterPacketEventToServer(CreateSPtr<SendAttackEventInfo>(HashCode("send attack"), direction));
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
			//direction.y = 0;

			//총알 위치 계산
			Vec3 pos = GetPosition();
			pos.y = 400.f;
			//총알 스폰
			const auto& bullet = CreateSPtr<Bullet>();
			bullet->SetPosition(pos);
			bullet->SetBlockingSphereRadius(10.f);
			bullet->SetVelocity(direction);//컨트롤러의 방향으로 총알을 발사한다.
			LOG_INFO("공격 패킷 보냄");	
			bullet->SetCollisionInfo(true, "bullet", { "enemy hit" }, true);
			bullet->SetOnCollisionResponse([bullet,this](const SPtr<SceneComponent>& component, const SPtr<Actor>& other_actor,

				const SPtr<SceneComponent>& other_component)
			{
				bullet->SetCollisionInfo(false, false, false);

				//LOG_INFO(component->GetName() + " " + other_actor->GetName() + " " + other_component->GetName());
				const auto& enemy = std::dynamic_pointer_cast<Enemy>(other_actor);
				if (enemy != nullptr)
				{
						float enemy_hp = enemy->GetHP();
						if (enemy_hp > 0)

							enemy->Hit(0,m_network_id);

							//LOG_INFO("충돌 부위 :" + other_component->GetName());
						bullet->SetActorState(eActorState::kDead);
				}
			});
			SpawnActor(bullet);
			m_is_fire = true;
		}
	}


	void RevivePlayer::Hit(float damage, int nw_id)

	{
		++m_hit_count;
		m_is_hitting = true;
		if (m_is_cheating == false)m_hp -= damage;
		else --m_hit_count;

		//LOG_INFO(nw_id);

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

