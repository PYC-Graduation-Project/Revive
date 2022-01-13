#include "stdafx.h"
#include "client/object/actor/pawn.h"
#include "client/object/actor/controller.h"
#include "client/input/input.h"

namespace client_fw
{
	Pawn::Pawn(const std::string& name)
		: Actor(eMobilityState::kMovable, name)
	{
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

	void Pawn::SetController(const SPtr<Controller>& controller)
	{
		m_controller = controller;
	}
	
}
