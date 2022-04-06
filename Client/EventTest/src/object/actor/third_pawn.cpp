#include <include/client_core.h>
#include <client/object/component/mesh/static_mesh_component.h>
#include <client/object/component/util/character_movement_component.h>
#include <client/object/component/util/camera_component.h>
#include <client/object/actor/player_controller.h>
#include "third_pawn.h"

namespace event_test
{
	ThirdPawn::ThirdPawn()
		: DefaultPawn("third pawn")
	{
		m_movement_component = CreateSPtr<CharacterMovementComponent>();
	}

	bool ThirdPawn::Initialize()
	{
		bool ret = true;

		ret &= Pawn::Initialize();
		ret &= AttachComponent(m_movement_component);
		//Character��� movement component�� character movement comp�� �����ϸ� �ȴ�. (�� cast ���ص� �ȴ�.)
		std::static_pointer_cast<CharacterMovementComponent>(m_movement_component)->UseOrientRotationToMovement(true);

		auto mesh = CreateSPtr<StaticMeshComponent>();
		ret &= mesh->SetMesh("../Contents/penguin.obj");
		mesh->SetLocalScale(10.0f);
		mesh->SetLocalRotation(quat::CreateQuaternionFromAxis(vec3::AXIS_Y, math::ToRadian(180.0f)));
		ret &= AttachComponent(mesh);

		auto camera = CreateSPtr<CameraComponent>();
		const auto& player_controller = std::dynamic_pointer_cast<PlayerController>(m_controller.lock());
		if (player_controller != nullptr)
			player_controller->SetPlayerCamera(camera);
		ret &= AttachComponent(camera);
		camera->UseControllerRotation(true);

	/*	SetUseControllerPitch(true);
		SetUseControllerYaw(true);
		SetUseControllerRoll(true);*/

		return ret;
	}

	void ThirdPawn::Shutdown()
	{
		DefaultPawn::Shutdown();
	}

	void ThirdPawn::Update(float delta_time)
	{
		DefaultPawn::Update(delta_time);
	}
}
