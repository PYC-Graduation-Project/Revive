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
#include "object/actor/weapon/pistol.h"
#include "object/actor/character/enemy.h"
#include "object/actor/character/revive_player.h"
#include "object/actor/projectile/projectile.h"
#include "object/actor/projectile/bullet.h"
#include "object/statemachine/state_machine.h"

namespace revive
{
	DefaultPlayer::DefaultPlayer(const std::string& name)
		:Pawn(name)
	{
		m_movement_component = CreateSPtr<CharacterMovementComponent>();
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

		m_skeletal_mesh_component->AddNotify("Attack End", "attack", 18,
			[this]() { m_is_attacking = false; /*LOG_INFO(m_is_attacking);*/ });
		m_skeletal_mesh_component->AddNotify("Hit End", "hit", 8,
			[this]() { m_is_hitting = false; /*LOG_INFO(m_is_attacking);*/ });

		ret &= AttachComponent(m_movement_component);
		m_movement_component->SetMaxSpeed(300.f);// Ÿ�� ũ�� 300 * 0.75(tile/s)  = 225 (�ٵ� �ʹ�������)
		m_movement_component->UseOrientRotationToMovement(true);

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
		m_hit_box->SetCollisionInfo(true, false, "player hit", { "enemy agro","stone","axe"}, false);
		ret &= AttachComponent(m_hit_box);

		SetScale(0.5f);

		return ret;
	}

	void DefaultPlayer::Shutdown()
	{
		m_skeletal_mesh_component = nullptr;
		m_player_fsm->Shutdown();
		m_player_fsm = nullptr;
		for (auto& weapon : m_weapon)
			weapon = nullptr;
	}

	void DefaultPlayer::Update(float delta_time)
	{
		m_player_fsm->Update();
	}

	void DefaultPlayer::RotatePlayerFromCameraDirection(Vec3& dest_direction)
	{
		Vec3 old_player_forward = GetForward();

		if (vec3::Dot(old_player_forward, dest_direction) == -1)//���纸�� ����� �ݴ�������� ȸ�� �� �ʹ��������ʰ� ȸ����Ű���� �ʿ��� �κ�
			old_player_forward += Vec3{ 0.02f,0.0f,-0.02f };

		float rotate_speed = 70.0f * 0.016f; //ȸ���ӵ� delta_time�����ؾ��ϳ� �����ϱ� 0.016����

		Vec3 curr_player_forward = /*old_player_forward +*/ dest_direction;//* rotate_speed;
		curr_player_forward = vec3::Normalize(curr_player_forward);

		float angle = vec3::BetweenAngle(curr_player_forward, vec3::AXIS_Z); //0~PI�� ��ȯ�� (radian)

		if (vec3::Cross(curr_player_forward, vec3::AXIS_Z, true).y > 0.0f) //0~2PI���� ������� if��
			angle = -angle;

		auto player_rot = quat::CreateQuaternionFromRollPitchYaw(0.0f, angle, 0.0f); //angle��ŭ Y�� ȸ��
		SetRotation(player_rot);
	}

	const float DefaultPlayer::GetVelocity() const
	{
		return m_movement_component->GetVelocity().Length();
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
		//ī�޶� ���� �������� ���� �����Ѵ�(ȸ��)
		Vec3 direction = m_controller.lock()->GetForward();
		direction.y = 0;
		RotatePlayerFromCameraDirection(direction);

		//�Ѿ� ����
		const auto& bullet = CreateSPtr<Bullet>();
		bullet->SetPosition(GetPosition() + Vec3{ 0.0f,50.0f,0.0f });
		bullet->SetBlockingSphereRadius(10.f);
		bullet->SetVelocity(m_controller.lock()->GetForward());//��Ʈ�ѷ��� �������� �Ѿ��� �߻��Ѵ�.
		bullet->SetCollisionInfo(true, "bullet", { "enemy hit" }, true);
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

				LOG_INFO("�浹 ���� :" + other_component->GetName());
				bullet->SetActorState(eActorState::kDead);
			}

		});
		SpawnActor(bullet);
	}

	void DefaultPlayer::Hit(int damage)
	{
		++m_hit_count;
		m_is_hitting = true;
		m_hp -= damage;
	}

	RevivePlayer::RevivePlayer(const std::string& name)
		: DefaultPlayer(name)
	{
		m_camera_component = CreateSPtr<RenderCameraComponent>("Follow Camera");
		m_blocking_sphere = CreateSPtr<SphereComponent>(40.0f,"Player Blocking Collision");
	}

	bool RevivePlayer::Initialize()
	{
		bool ret = DefaultPlayer::Initialize();

		//Notify ����� ����� �ִϸ��̼��� �̸� ����Ѵ� 
		//Notify �̸�,�ִϸ��̼� �̸�, Ư�� Ű������, Ư�� �ð��� ������ �Լ�
		//Notify �̸��� �𸮾�ó�� �־��ֱ� ������, ���� ��������� �ʰ� �ִ�.
		

		ret &= AttachComponent(m_blocking_sphere);
		m_blocking_sphere->SetLocalPosition(Vec3{ 0.0f,m_blocking_sphere->GetExtents().y,0.0f });
		m_blocking_sphere->SetCollisionInfo(true, true, "player", { "base","wall","enemy agro","enemy attack"}, true);
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
		DefaultPlayer::Update(delta_time);
		FixYPosition();
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
		//�׽�Ʈ�� ���Ű
		//�´� ���� �ٽ� �� ���� �� �ִ�.
		RegisterPressedEvent("Hit", { {eKey::kP} },
			[this]()->bool { ++m_hit_count; m_is_hitting = true; DecrementHP();  return true; });
		RegisterPressedEvent("Cheat Mode", { {eKey::kC} },
			[this]()->bool {  m_is_cheating = !m_is_cheating; 
		if (m_is_cheating)LOG_INFO("Cheat Mode On"); 
		else LOG_INFO("Cheat Mode Off");
		return true; });

		//����
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

	void RevivePlayer::Hit(int damage)
	{
		++m_hit_count;
		m_is_hitting = true;
		if (m_is_cheating == false)m_hp -= damage;
		else --m_hit_count;
	}

	void RevivePlayer::MinPitch()
	{
		Vec3 rot = quat::QuaternionToEuler(m_controller.lock()->GetRotation());
		if (math::ToDegrees(rot.x) < 5) //��Ʈ�ѷ����� SetRotation(rot);�� �����ִ°� ���� ���� ���
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
		LOG_INFO("{0}�� {1}�� �浹�ߴ�", component->GetOwner().lock()->GetName(), other_component->GetName());
	}
	
	

}

