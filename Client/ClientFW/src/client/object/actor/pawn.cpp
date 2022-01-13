#include "stdafx.h"
#include "client/object/actor/pawn.h"
#include "client/object/actor/controller.h"
#include "client/object/actor/player_controller.h"
#include "client/input/input.h"

namespace client_fw
{
	Pawn::Pawn(const std::string& name)
		: Actor(eMobilityState::kMovable, name)
	{
	}

	void Pawn::UpdateActor(float delta_time)
	{
		if (m_controller.expired() == false)
		{
			m_is_need_update = true;
			if (m_controller.lock()->IsUpdated())
			{
				Actor::UpdateActor(delta_time);
				m_is_need_update = false;
			}
		}
		else
		{
			Actor::UpdateActor(delta_time);
		}
	}

	void Pawn::Update(float delta_time)
	{
		if (m_controller.expired() == false)
		{
			const auto& controller = m_controller.lock();
			if (IsUseControllerPitch())
			{

			}
			if (IsUseControllerYaw())
			{
				Vec3 pawn_forward = GetForward();
				pawn_forward.y = 0.0f;
				pawn_forward.Normalize();
				Vec3 cont_forward = controller->GetForward();
				cont_forward.y = 0.0f;
				cont_forward.Normalize();

				Quaternion rot = GetRotation();
				rot *= quat::CreateQuaternionFromNormal(vec3::AXIS_Y, vec3::BetweenAngle(pawn_forward, cont_forward));
				SetRotation(rot);
			}
			if (IsUseControllerRoll())
			{

			}
		}
		
	}

	void Pawn::RegisterPressedEvent(const std::string& name, std::vector<EventKeyInfo>&& keys, 
		const std::function<bool()>& func, bool consumption)
	{
		std::string event_name = m_name + " : " + name;
		if (Input::RegisterPressedEvent(event_name, std::move(keys), func, consumption, eInputOwnerType::kPawn))
			RegisterInputEvent(event_name);
	}

	void Pawn::RegisterReleasedEvent(const std::string& name, std::vector<EventKeyInfo>&& keys,
		const std::function<bool()>& func, bool consumption)
	{
		std::string event_name = m_name + " : " + name;
		if (Input::RegisterReleasedEvent(event_name, std::move(keys), func, consumption, eInputOwnerType::kPawn))
			RegisterInputEvent(event_name);
	}

	void Pawn::RegisterAxisEvent(const std::string& name, std::vector<AxisEventKeyInfo>&& keys, 
		const std::function<bool(float)>& func, bool consumption)
	{
		std::string event_name = m_name + " : " + name;
		if (Input::RegisterAxisEvent(event_name, std::move(keys), func, consumption, eInputOwnerType::kPawn))
			RegisterInputEvent(event_name);
	}

	bool Pawn::AddControllerPitchInput(float value)
	{
		if (m_controller.expired() == false)
		{
			const auto& player_controller = std::dynamic_pointer_cast<PlayerController>(m_controller.lock());
			player_controller->AddPitchInput(value);
			return true;
		}
		return false;
	}

	bool Pawn::AddControllerYawInput(float value)
	{
		if (m_controller.expired() == false)
		{
			const auto& player_controller = std::dynamic_pointer_cast<PlayerController>(m_controller.lock());
			player_controller->AddYawInput(value);
			return true;
		}
		return false;
	}

	bool Pawn::AddControllerRollInput(float value)
	{
		if (m_controller.expired() == false)
		{
			const auto& player_controller = std::dynamic_pointer_cast<PlayerController>(m_controller.lock());
			player_controller->AddRollInput(value);
			return true;
		}
		return false;
	}

	void Pawn::SetController(const SPtr<Controller>& controller)
	{
		m_controller = controller;
	}
	
}
