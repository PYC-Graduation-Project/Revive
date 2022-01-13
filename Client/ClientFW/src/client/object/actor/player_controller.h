#pragma once
#include "client/object/actor/controller.h"

namespace client_fw
{
	class PlayerController : public Controller
	{
	public:
		PlayerController(const std::string& name = "player controller");
		virtual ~PlayerController() = default;

		void AddPitchInput(float value);
		void AddYawInput(float value);
		void AddRollInput(float value);

	private:
		float m_pitch_speed_scale = 2.5f;
		float m_yaw_speed_scale = 2.5f;
		float m_roll_speed_scale = 0.5f;

	public:
		void SetPitchSpeedScale(float scale) { m_pitch_speed_scale = scale; }
		void SetYawSpeedScale(float scale) { m_yaw_speed_scale = scale; }
		void SetRollSpeedScale(float scale) { m_roll_speed_scale = scale; }
	};
}


