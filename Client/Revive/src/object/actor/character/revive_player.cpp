#include <include/client_core.h>
#include <client/object/component/core/scene_component.h>
#include <client/object/component/util/simple_movement_component.h>
#include <client/object/component/mesh/static_mesh_component.h>
#include <client/object/component/mesh/skeletal_mesh_component.h>
#include <client/object/component/render/sphere_component.h>
#include <client/object/component/render/box_component.h>
#include <client/object/component/util/render_camera_component.h>
#include <client/object/component/util/character_movement_component.h>
#include <client/object/actor/player_controller.h>
#include <client/object/actor/core/actor.h>
#include <client/input/input.h>
#include "object/actor/character/enemy.h"
#include "object/actor/character/revive_player.h"
#include "object/actor/projectile/projectile.h"
#include "object/actor/projectile/bullet.h"
#include "object/statemachine/state_machine.h"

namespace revive
{
	RevivePlayer::RevivePlayer(const std::string& name)
		: Pawn(name)
	{
		m_camera_component = CreateSPtr<RenderCameraComponent>("Follow Camera");
		m_movement_component = CreateSPtr<CharacterMovementComponent>();
		m_skeletal_mesh_component = CreateSPtr<SkeletalMeshComponent>();
		m_player_fsm = CreateSPtr<PlayerFSM>();
		m_mesh_path = "Contents/violet.rev";
		m_blocking_sphere = CreateSPtr<SphereComponent>(40.0f,"Player Blocking Collision");
	}

	bool RevivePlayer::Initialize()
	{
		bool ret = true;

		ret &= Pawn::Initialize();

		ret &= AttachComponent(m_movement_component);
		m_movement_component->SetMaxSpeed(300.f);// 타일 크기 300 * 0.75(tile/s)  = 225 (근데 너무느리다)
		m_movement_component->UseOrientRotationToMovement(true);

		ret &= m_skeletal_mesh_component->SetMesh(m_mesh_path);
		ret &= AttachComponent(m_skeletal_mesh_component);
		m_skeletal_mesh_component->SetLocalRotation(math::ToRadian(-90.0f), math::ToRadian(180.0f), 0.0f);
		m_skeletal_mesh_component->SetLocalScale(0.01f);

		//Notify 기능을 사용할 애니메이션을 미리 등록한다 
		//Notify 이름,애니메이션 이름, 특정 키프레임, 특정 시간에 실행할 함수
		//Notify 이름을 언리얼처럼 넣어주긴 했으나, 정작 사용하지는 않고 있다.
		m_skeletal_mesh_component->AddNotify("Attack End", "attack", 18, 
			[this]() { m_is_attacking = false; /*LOG_INFO(m_is_attacking);*/ });
		m_skeletal_mesh_component->AddNotify("Hit End", "hit", 8, 
			[this]() { m_is_hitting = false; /*LOG_INFO(m_is_attacking);*/ });

		m_player_fsm->Initialize(SharedFromThis());
		
		ret &= AttachComponent(m_blocking_sphere);
		m_blocking_sphere->SetLocalPosition(Vec3{ 0.0f,m_blocking_sphere->GetExtents().y,0.0f });
		m_blocking_sphere->SetCollisionInfo(true, true, "player", { "base","wall","enemy agro","enemy attack"}, true);
		m_blocking_sphere->OnCollisionResponse([this](const SPtr<SceneComponent>& component, const SPtr<Actor>& other_actor,
				const SPtr<SceneComponent>& other_component) {
			FixYPosition();
			//LOG_INFO("Player sphere component {0} Player Position {1} Extents {2}", m_blocking_sphere->GetWorldPosition(), this->GetPosition(), m_blocking_sphere->GetExtents());
			});

		//Hit Box
		SPtr<BoxComponent> hit_box_1 = CreateSPtr<BoxComponent>(Vec3{40.f,40.f,40.f});
		hit_box_1->SetName("head hit box");
		hit_box_1->SetLocalPosition(Vec3{ 0.0f,130.f,0.f });
		m_hit_boxes.emplace_back(hit_box_1);
		SPtr<BoxComponent> hit_box_2 = CreateSPtr<BoxComponent>(Vec3{ 25.f,30.f,20.f });
		hit_box_2->SetName("body hit box");
		hit_box_2->SetLocalPosition(Vec3{ 0.0f,60.f,0.f });
		m_hit_boxes.emplace_back(hit_box_2);
		SPtr<BoxComponent> hit_box_3 = CreateSPtr<BoxComponent>(Vec3{ 8.f,15.f,15.f });
		hit_box_3->SetName("leg hit box");
		hit_box_3->SetLocalPosition(Vec3{ 10.0f,15.f,-5.f });
		m_hit_boxes.emplace_back(hit_box_3);
		SPtr<BoxComponent> hit_box_4 = CreateSPtr<BoxComponent>(Vec3{ 8.f,15.f,15.f });
		hit_box_4->SetName("leg hit box");
		hit_box_4->SetLocalPosition(Vec3{ -10.0f,15.f,-5.f });
		m_hit_boxes.emplace_back(hit_box_4);

		for (const auto& hit_box : m_hit_boxes)
		{
			hit_box->SetCollisionInfo(true, false, "player hit", { "stone","axe"}, false);
			ret &= AttachComponent(hit_box);
		}

		const auto& player_controller = std::dynamic_pointer_cast<PlayerController>(m_controller.lock());
		if (player_controller != nullptr)
			player_controller->SetPlayerCamera(m_camera_component);
		ret &= AttachComponent(m_camera_component);
		m_camera_component->UseControllerRotation(true);
		m_camera_component->SetLocalPosition(Vec3(0.0f, 200.0f, -500.0f));

		RegisterEvent();
		
		SetPosition(Vec3{ 2400.0f,300.0f,3400.0f });
		SetScale(0.5f);

		SetUseControllerPitch(false);
		SetUseControllerYaw(false);
		SetUseControllerRoll(false);

		return ret;
	}  

	void RevivePlayer::Update(float delta_time)
	{
		Pawn::Update(delta_time);
		FixYPosition();
		m_player_fsm->Update();
	}

	void RevivePlayer::Shutdown()
	{
		m_movement_component = nullptr;
		m_skeletal_mesh_component = nullptr;
		m_blocking_sphere = nullptr;
		m_camera_component = nullptr;
		m_player_fsm->Shutdown();
		m_player_fsm = nullptr;
	}
	

	const float RevivePlayer::GetVelocity() const
	{
		return m_movement_component->GetVelocity().Length(); 
	}

	void RevivePlayer::SetAnimation(const std::string& animation_name, bool looping)
	{
		m_skeletal_mesh_component->SetAnimation(animation_name, looping);
	}

	void RevivePlayer::SetAnimationSpeed(float speed)
	{
		m_skeletal_mesh_component->SetAnimationSpeed(speed);
	}

	void RevivePlayer::SetMeshPosition(const Vec3& pos)
	{
		m_skeletal_mesh_component->SetLocalPosition(pos);
	}

	void RevivePlayer::Attack()
	{
		//카메라가 보는 방향으로 총을 쏴야한다(회전)
		Vec3 direction = m_controller.lock()->GetForward();
		direction.y = 0;
		RotatePlayerFromCameraDirection(direction);

		//총알 스폰
		const auto& bullet = CreateSPtr<Bullet>();
		bullet->SetPosition(GetPosition() + Vec3{0.0f,50.0f,0.0f});
		bullet->SetBlockingSphereRadius(10.f);
		bullet->SetVelocity(m_controller.lock()->GetForward());//컨트롤러의 방향으로 총알을 발사한다.
		bullet->SetCollisionInfo(true, "bullet", { "enemy hit"}, true);
		bullet->SetOnCollisionResponse([bullet](const SPtr<SceneComponent>& component, const SPtr<Actor>& other_actor,
		const SPtr<SceneComponent>& other_component)
		{
			LOG_INFO(component->GetName() + " " + other_actor->GetName() + " " + other_component->GetName());
			const auto& enemy = std::dynamic_pointer_cast<Enemy>(other_actor);
			if (enemy != nullptr)
			{
				int enemy_hp = enemy->GetHP();
				if (enemy_hp > 0)
					enemy->Hit();

				LOG_INFO("충돌 부위 :" + other_component->GetName());
				bullet->SetActorState(eActorState::kDead);
			}
			
		});
		SpawnActor(bullet);

	}

	void RevivePlayer::Hit(int damage)
	{
		++m_hit_count; 
		m_is_hitting = true; 
		m_hp -= damage;
	}

	void RevivePlayer::RegisterEvent()
	{
		//테스트용 명령키
		//맞는 도중 다시 또 맞을 수 있다.
		RegisterPressedEvent("Hit", { {eKey::kP} },
			[this]()->bool { ++m_hit_count; m_is_hitting = true; DecrementHP();  return true; });

		//공격
		RegisterPressedEvent("attack", { {eKey::kLButton} },
			[this]()->bool {  
			bool ret = false;
			if(m_is_dying == false)
				if (m_is_attacking == false)
					ret = m_is_attacking = true; 
			return ret;
		});
		
	}

	void RevivePlayer::AddMovementInput(const Vec3& direction, float scale)
	{
		m_movement_component->AddInputVector(Vec3{ direction.x,0.0f,direction.z } * scale);
	}

	void RevivePlayer::RotatePlayerFromCameraDirection(Vec3& dest_direction)
	{
		Vec3 old_player_forward = GetForward();

		if (vec3::Dot(old_player_forward, dest_direction) == -1)//현재보는 방향과 반대방향으로 회전 시 너무느리지않게 회전시키려면 필요한 부분
			old_player_forward += Vec3{ 0.02f,0.0f,-0.02f };

		float rotate_speed = 70.0f * 0.016f; //회전속도 delta_time을곱해야하나 없으니까 0.016곱함

		Vec3 curr_player_forward = /*old_player_forward +*/ dest_direction;//* rotate_speed;
		curr_player_forward = vec3::Normalize(curr_player_forward);

		float angle = vec3::BetweenAngle(curr_player_forward, vec3::AXIS_Z); //0~PI만 반환함 (radian)

		if (vec3::Cross(curr_player_forward, vec3::AXIS_Z, true).y > 0.0f) //0~2PI값을 얻기위한 if문
			angle = -angle;

		auto player_rot = quat::CreateQuaternionFromRollPitchYaw(0.0f, angle, 0.0f); //angle만큼 Y축 회전
		SetRotation(player_rot);
	}

	void RevivePlayer::MinPitch()
	{
		Vec3 rot = quat::QuaternionToEuler(m_controller.lock()->GetRotation());
		if (math::ToDegrees(rot.x) < 5) //컨트롤러에서 SetRotation(rot);를 안해주는게 제일 좋은 방법
		{
			rot.x += math::ToRadian(5.0f) - rot.x;
			m_controller.lock()->SetRotation(quat::CreateQuaternionFromRollPitchYaw(rot.x, rot.y, rot.z));
		}
	}

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

