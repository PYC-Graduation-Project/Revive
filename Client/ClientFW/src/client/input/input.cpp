#include "stdafx.h"
#include "client/input/input_manager.h"

namespace client_fw
{
	bool Input::IsKeyHoldDown(EKey key)
	{
		return s_input_manager->IsKeyHoldDown(ToUnderlying(key));
	}

	bool Input::IsKeyHoldDown(EAdditionalKey key)
	{
		return s_input_manager->IsKeyHoldDown(ToUnderlying(key));
	}

	bool Input::IsKeyPressed(EKey key)
	{
		return s_input_manager->IsKeyPressed(ToUnderlying(key));
	}

	bool Input::IsKeyReleased(EKey key)
	{
		return s_input_manager->IsKeyReleased(ToUnderlying(key));
	}

	bool Input::IsNotKeyHoldDown(EKey key)
	{
		return s_input_manager->IsNotKeyHoldDown(ToUnderlying(key));
	}

	const IVec2& Input::GetMousePosition()
	{
		return s_input_manager->GetMousePosition();
	}

	const IVec2 Input::GetRelativeMousePosition()
	{
		return s_input_manager->GetRelativeMoustPosition();
	}

	void Input::ConsumeKey(EKey key)
	{
		s_input_manager->ConsumeKey(ToUnderlying(key));
	}

	bool Input::IsConsumedKey(EKey key)
	{
		return s_input_manager->IsConsumedKey(ToUnderlying(key));
	}

	void Input::SetHideCursor(bool hide)
	{
		if (hide != IsHideCursor())
			s_input_manager->SetHideCursor(hide);
	}

	bool Input::IsHideCursor()
	{
		return s_input_manager->IsHideCursor();
	}

	void Input::SetClipCursor(bool clip)
	{
		if (clip != IsClipCursor())
			s_input_manager->SetClipCursor(clip);
	}

	bool Input::IsClipCursor()
	{
		return s_input_manager->IsClipCursor();
	}
}