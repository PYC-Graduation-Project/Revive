#include "stdafx.h"
#include "client/input/input.h"
#include "client/input/input_event_manager.h"

namespace client_fw
{
	InputEventInfo::InputEventInfo(std::string_view event_name, bool is_comsume)
		: m_event_name(event_name), m_is_comsume_input(is_comsume)
	{
	}

	ActionEventInfo::ActionEventInfo(std::string_view event_name, bool is_comsume,
		std::vector<EventKeyInfo>&& event_keys, const std::function<void()>& event_func)
		: InputEventInfo(event_name, is_comsume)
		, m_event_keys(event_keys), m_event_func(event_func)
	{
	}

	PressedEventInfo::PressedEventInfo(std::string_view event_name, bool is_comsume,
		std::vector<EventKeyInfo>&& event_keys, const std::function<void()>& event_func)
		: ActionEventInfo(event_name, is_comsume, std::move(event_keys), event_func)
	{
	}

	void PressedEventInfo::ExecuteEvent() const
	{
		for (const auto event_key : m_event_keys)
		{
			if (Input::IsKeyPressed(event_key.key) && Input::IsConsumedKey(event_key.key) == false &&
				std::all_of(event_key.additional_keys.cbegin(), event_key.additional_keys.cend(),
					[this](EAdditionalKey add_key)
					{
						return Input::IsKeyHoldDown(add_key);
					}))
			{
				m_event_func();
				if (m_is_comsume_input)
					Input::ConsumeKey(event_key.key);
				break;
			}
		}
	}

	ReleasedEventInfo::ReleasedEventInfo(std::string_view event_name, bool is_comsume,
		std::vector<EventKeyInfo>&& event_keys, const std::function<void()>& event_func)
		: ActionEventInfo(event_name, is_comsume, std::move(event_keys), event_func)
	{
	}

	void ReleasedEventInfo::ExecuteEvent() const
	{
	}

	AxisEventInfo::AxisEventInfo(std::string_view event_name, bool is_comsume,
		std::vector<AxisEventKeyInfo>&& event_keys, const std::function<void(float)>& event_func)
		: InputEventInfo(event_name, is_comsume)
		, m_event_keys(event_keys), m_event_func(event_func)
	{
	}

	void AxisEventInfo::ExecuteEvent() const
	{
	}

	InputEventManager::InputEventManager()
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
