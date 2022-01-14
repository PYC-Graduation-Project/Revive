#pragma once
#include "client/object/actor/controller.h"

namespace client_fw
{
	class PlayerCameraManager;
	class CameraComponent;

	class PlayerController : public Controller
	{
	public:
		PlayerController(const std::string& name = "player controller");
		virtual ~PlayerController();

		virtual bool Initialize() override;
		virtual void Shutdown() override;

		void AddPitchInput(float value);
		void AddYawInput(float value);
		void AddRollInput(float value);

	private:
		static PlayerController* s_player_controller;
		SPtr<CameraComponent> m_camera_component;
		float m_pitch_speed_scale = 2.5f;
		float m_yaw_speed_scale = 2.5f;
		float m_roll_speed_scale = 0.5f;

	public:
		const SPtr<CameraComponent>& GetPlayerCamera() const { return m_camera_component; }
		void SetPlayerCamera(const SPtr<CameraComponent>& camera_comp) { m_camera_component = camera_comp; }
		void SetPitchSpeedScale(float scale) { m_pitch_speed_scale = scale; }
		void SetYawSpeedScale(float scale) { m_yaw_speed_scale = scale; }
		void SetRollSpeedScale(float scale) { m_roll_speed_scale = scale; }
	};
}


