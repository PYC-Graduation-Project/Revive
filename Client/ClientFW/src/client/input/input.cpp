#include "stdafx.h"
#include "client/input/input.h"
#include "client/input/input_manager.h"
#include "client/event/inputevent/input_event_info.h"
#include "client/event/inputevent/input_event_manager.h"
#include "client/event/inputevent/input_event_system.h"

namespace client_fw
{
	bool Input::IsKeyHoldDown(eKey key)
	{
		return s_input_event_system->GetInputManager()->IsKeyHoldDown(ToUnderlying(key));
	}

	bool Input::IsKeyHoldDown(eAdditionalKey key)
	{
		return s_input_event_system->GetInputManager()->IsKeyHoldDown(ToUnderlying(key));
	}

	bool Input::IsKeyPressed(eKey key)
	{
		return s_input_event_system->GetInputManager()->IsKeyPressed(ToUnderlying(key));
	}

	bool Input::IsKeyReleased(eKey key)
	{
		return s_input_event_system->GetInputManager()->IsKeyReleased(ToUnderlying(key));
	}

	bool Input::IsNotKeyHoldDown(eKey key)
	{
		return s_input_event_system->GetInputManager()->IsNotKeyHoldDown(ToUnderlying(key));
	}

	const IVec2& Input::GetMousePosition()
	{
		return s_input_event_system->GetInputManager()->GetMousePosition();
	}

	const IVec2 Input::GetRelativeMousePosition()
	{
		return s_input_event_system->GetInputManager()->GetRelativeMoustPosition();
	}

	void Input::ConsumeKey(eKey key)
	{
		s_input_event_system->GetInputManager()->ConsumeKey(ToUnderlying(key));
	}

	bool Input::IsConsumedKey(eKey key)
	{
		return s_input_event_system->GetInputManager()->IsConsumedKey(ToUnderlying(key));
	}

	void Input::SetHideCursor(bool hide)
	{
		if (hide != IsHideCursor())
			s_input_event_system->GetInputManager()->SetHideCursor(hide);
	}

	bool Input::IsHideCursor()
	{
		return s_input_event_system->GetInputManager()->IsHideCursor();
	}

	void Input::SetClipCursor(bool clip)
	{
		if (clip != IsClipCursor())
			s_input_event_system->GetInputManager()->SetClipCursor(clip);
	}

	bool Input::IsClipCursor()
	{
		return s_input_event_system->GetInputManager()->IsClipCursor();
	}

	bool Input::RegisterPressedEvent(std::string_view name, std::vector<EventKeyInfo>&& keys,
		const std::function<void()>& func, bool consumption, eInputOwnerType type)
	{
		return s_input_event_system->GetInputEventManager()->RegisterEvent(
			CreateUPtr<PressedEventInfo>(name, consumption, std::move(keys), func), type
		);
	}

	bool Input::RegisterReleasedEvent(std::string_view name, std::vector<EventKeyInfo>&& keys,
		const std::function<void()>& func, bool consumption, eInputOwnerType type)
	{
		return s_input_event_system->GetInputEventManager()->RegisterEvent(
			CreateUPtr<ReleasedEventInfo>(name, consumption, std::move(keys), func), type
		);
	}

	bool Input::RegisterAxisEvent(std::string_view name, std::vector<AxisEventKeyInfo>&& keys,
		const std::function<void(float)>& func, bool consumption, eInputOwnerType type)
	{
		return s_input_event_system->GetInputEventManager()->RegisterEvent(
			CreateUPtr<AxisEventInfo>(name, consumption, std::move(keys), func), type
		);
	}

	void Input::UnregisterInputEvent(std::string_view name)
	{
		s_input_event_system->GetInputEventManager()->UnregisterEvent(name);
	}

	void Input::SetInputMode(eInputMode input_mode)
	{
		s_input_event_system->GetInputEventManager()->SetInputMode(input_mode);
	}
}