#include "stdafx.h"
#include "client/object/actor/player_controller.h"
#include "client/input/input.h"
#include "client/object/component/util/camera_component.h"
#include "client/object/actor/pawn.h"

namespace client_fw
{
	PlayerController* PlayerController::s_player_controller = nullptr;

	PlayerController::PlayerController(const std::string& name)
		: Controller(name)
	{
		m_camera_component = CreateSPtr<CameraComponent>();
	}

	PlayerController::~PlayerController()
	{
	}

	bool PlayerController::Initialize()
	{
		if (s_player_controller == nullptr)
		{
			s_player_controller = this;
			if (m_controlled_pawn != nullptr)
			{
				m_camera_component->SetActive();
				return m_controlled_pawn->AttachComponent(m_camera_component);
			}
			return true;
		}
		return false;
	}

	void PlayerController::Shutdown()
	{
		if (s_player_controller == this)
		{
			s_player_controller = nullptr;
		}
	}

	void PlayerController::AddPitchInput(float value)
	{
	}

	void PlayerController::AddYawInput(float value)
	{
		IVec2 relative_pos = Input::GetRelativeMousePosition();
		Quaternion rot = GetRotation();
		rot *= quat::CreateQuaternionFromNormal(vec3::AXIS_Y, math::ToRadian(value * m_yaw_speed_scale * relative_pos.x));
		SetRotation(rot);
	}

	void PlayerController::AddRollInput(float value)
	{
	}
}
