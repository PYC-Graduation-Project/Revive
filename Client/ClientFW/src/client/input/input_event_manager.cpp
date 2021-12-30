#include "stdafx.h"
#include "client/input/input_event_manager.h"

namespace client_fw
{
	InputEventManager::InputEventManager()
	{
	}

	void InputEventManager::ProcessInput()
	{
		for (const auto& event : m_pressed_events)
		{
			for (const auto event_key : event.event_keys)
			{
				if (Input::IsKeyPressed(event_key.key) &&
					(
						event_key.additional_key == EAdditionalKey::kNone ||
						Input::IsKeyHoldDown(ToUnderlying(event_key.additional_key))
						)
					&& Input::IsConsumedKey(event_key.key) == false)
				{
					event.event_func();
					if (event.comsume_input)
						Input::ConsumeKey(event_key.key);
					break;
				}
			}
		}
	}

	void InputEventManager::RegisterPressedEvent(std::string_view name, std::vector<EventKeyInfo>&& keys,
		const std::function<void()>& func, bool consumption)
	{
		if (m_event_names.find(name.data()) == m_event_names.cend())
		{
			m_pressed_events.emplace_back(InputEventInfo{ name, keys, consumption, func });
		}
	}
}
