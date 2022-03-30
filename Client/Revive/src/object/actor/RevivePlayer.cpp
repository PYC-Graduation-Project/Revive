#include <include/client_core.h>
#include <client/object/component/core/scene_component.h>
#include <client/object/component/util/simple_movement_component.h>
#include <client/object/component/mesh/static_mesh_component.h>
#include <client/object/component/mesh/skeletal_mesh_component.h>
#include <client/object/component/render/sphere_component.h>
#include <client/object/component/render/box_component.h>
#include <client/object/actor/core/actor.h>
#include <client/input/input.h>
#include "object/component/follow_camera.h"
#include "object/actor/RevivePlayer.h"

namespace revive
{
	RevivePlayer::RevivePlayer(const std::string& name)
		: Pawn(name)
	{
		m_camera_component = CreateSPtr<FollowCamera>("Follow Camera", eCameraUsage::kBasic);
		m_movement_component = CreateSPtr<SimpleMovementComponent>();
		m_skeletal_mesh_component = CreateSPtr<SkeletalMeshComponent>();
		m_sphere_component = CreateSPtr<SphereComponent>(30.0f);
		m_mesh_path = "Contents/violet.rev";
	}

	bool RevivePlayer::Initialize()
	{
		bool ret = true;

		ret &= Pawn::Initialize();
		ret &= AttachComponent(m_movement_component);

		ret &= m_skeletal_mesh_component->SetMesh(m_mesh_path);
		m_skeletal_mesh_component->SetLocalRotation(80.0f, 185.0f, 0.0f);
		ret &= AttachComponent(m_skeletal_mesh_component);
		m_skeletal_mesh_component->SetAnimation("idle");//SetAnimation도 bool값으로 성공여부를 리턴하면 좋을 듯
		
		m_sphere_component->SetCollisionInfo(true, false, "default", { "default" }, true);
		ret &= AttachComponent(m_sphere_component);

		
		ret &= AttachComponent(m_camera_component);
		

		RegisterAxisEvent("move forward", { AxisEventKeyInfo{eKey::kW, 1.0f}, AxisEventKeyInfo{eKey::kS, -1.0f} },
			[this](float axis)->bool {
			AddMovementInput(m_controller.lock()->GetForward(),axis); return true; });
		RegisterAxisEvent("move right", { AxisEventKeyInfo{eKey::kD, 1.0f}, AxisEventKeyInfo{eKey::kA, -1.0f} },
			[this](float axis)->bool { AddMovementInput(m_controller.lock()->GetRight(), axis); return true; });
		/*RegisterAxisEvent("move up", { AxisEventKeyInfo{eKey::kE, 1.0f}, AxisEventKeyInfo{eKey::kQ, -1.0f} },
			[this](float axis)->bool { AddMovementInput(GetUp(), axis); return true; });*/

		RegisterAxisEvent("turn", { AxisEventKeyInfo{eKey::kXMove, 1.0f} },
			[this](float axis)->bool {
			IVec2 relative_pos = Input::GetRelativeMousePosition();
			return AddControllerYawInput(axis * relative_pos.x);
		});
		RegisterAxisEvent("look up", { AxisEventKeyInfo{eKey::kYMove, 1.0f} },
			[this](float axis)->bool {
			IVec2 relative_pos = Input::GetRelativeMousePosition();
			MinPitch();
			return AddControllerPitchInput(axis * relative_pos.y);
			
		});

		SetUseControllerPitch(false);
		SetUseControllerYaw(false);
		SetUseControllerRoll(false);

		return ret;
	}  

	void RevivePlayer::Shutdown()
	{
		m_movement_component = nullptr;
		m_skeletal_mesh_component = nullptr;
	}

	void RevivePlayer::Update(float delta_time)
	{
		Pawn::Update(delta_time);
		Vec3 current_position = GetPosition();
		if (current_position.y < 300)//땅의 높이
			current_position.y = 300;
		
		SetPosition(current_position);
	}

	void RevivePlayer::AddMovementInput(Vec3& direction, float scale)
	{
		direction.y = 0;
		RotatePlayerFromCameraDirection(direction * scale);
		m_movement_component->AddInputVector(direction * scale);
	}

	void RevivePlayer::RotatePlayerFromCameraDirection(Vec3& dest_direction)
	{
		Vec3 old_player_forward = GetForward();

		if (vec3::Dot(old_player_forward, dest_direction) == -1)//현재보는 방향과 반대방향으로 회전 시 너무느리지않게 회전시키려면 필요한 부분
			old_player_forward += Vec3{ 0.02f,0.0f,-0.02f };

		float rotate_speed = 10.0f * 0.016f; //회전속도 delta_time을곱해야하나 없으니까 0.016곱함

		Vec3 curr_player_forward = old_player_forward + dest_direction * rotate_speed;
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

