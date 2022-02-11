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
				m_camera_component->SetOwnerController(shared_from_this());
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
		m_pitch_speed = math::ToRadian(value * m_pitch_speed_scale);
	}

	void PlayerController::AddYawInput(float value)
	{
		m_yaw_speed = math::ToRadian(value * m_yaw_speed_scale);
	}

	void PlayerController::AddRollInput(float value)
	{
		m_roll_speed = math::ToRadian(value * m_roll_speed_scale);
	}
}
