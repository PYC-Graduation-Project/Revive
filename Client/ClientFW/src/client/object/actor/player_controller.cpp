#include "stdafx.h"
#include "client/object/actor/player_controller.h"
#include "client/input/input.h"

namespace client_fw
{
	PlayerController::PlayerController(const std::string& name)
	{
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
