#include "stdafx.h"
#include "client/input/input.h"
#include "client/event/inputevent/input_event_info.h"
#include "client/event/inputevent/input_event_manager.h"

namespace client_fw
{
	InputEventManager::InputEventManager()
	{
	}

	InputEventManager::~InputEventManager()
	{
	}

	void InputEventManager::ExecuteEvent()
	{
		for (const auto& event : m_application_events)
		{
			event->ExecuteEvent();
		}
		
	}

	void InputEventManager::RegisterEvent(UPtr<InputEventInfo>&& event_info, EInputOwnerType type)
	{
		std::string_view event_name = event_info->GetEventName();

		if (m_event_names.find(event_name) == m_event_names.cend())
		{
			switch (type)
			{
			case EInputOwnerType::kApplication:
				m_application_events.emplace_back(std::move(event_info));
				break;
			case EInputOwnerType::kLevel:
				break;
			case EInputOwnerType::kActor:
				break;
			case EInputOwnerType::kPawn:
				break;
			default:
				break;
			}
			m_event_names.emplace(event_name);
		}
		else
		{
			LOG_WARN("Event \"{0}\" is already exist", event_name);
		}
	}
}
