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

	void Pawn::RegisterPressedEvent(std::string_view name, std::vector<EventKeyInfo>&& keys, 
		const std::function<bool()>& func, bool consumption)
	{
		if (Input::RegisterPressedEvent(name, std::move(keys), func, consumption, eInputOwnerType::kActor))
			RegisterInputEvent(name);
	}

	void Pawn::RegisterReleasedEvent(std::string_view name, std::vector<EventKeyInfo>&& keys,
		const std::function<bool()>& func, bool consumption)
	{
		if (Input::RegisterReleasedEvent(name, std::move(keys), func, consumption, eInputOwnerType::kActor))
			RegisterInputEvent(name);
	}

	void Pawn::RegisterAxisEvent(std::string_view name, std::vector<AxisEventKeyInfo>&& keys, 
		const std::function<bool(float)>& func, bool consumption)
	{
		if (Input::RegisterAxisEvent(name, std::move(keys), func, consumption, eInputOwnerType::kActor))
			RegisterInputEvent(name);
	}

	void Pawn::SetController(const SPtr<Controller>& controller)
	{
		m_controller = controller;
	}
	
}
