#include "stdafx.h"
#include "client/input/input_manager.h"

namespace client_fw
{
	bool Input::IsKeyHoldDown(UINT key)
	{
		return s_input_manager->IsKeyHoldDown(key);
	}

	bool Input::IsKeyPressed(UINT key)
	{
		return s_input_manager->IsKeyPressed(key);
	}

	bool Input::IsKeyReleased(UINT key)
	{
		return s_input_manager->IsKeyReleased(key);
	}

	bool Input::IsNotKeyHoldDown(UINT key)
	{
		return s_input_manager->IsNotKeyHoldDown(key);
	}

	const IVec2& Input::GetMousePosition()
	{
		return s_input_manager->GetMousePosition();
	}

	const IVec2 Input::GetRelativeMousePosition()
	{
		return s_input_manager->GetRelativeMoustPosition();
	}

	void Input::ConsumeKey(UINT key)
	{
		s_input_manager->ConsumeKey(key);
	}

	bool Input::IsConsumedKey(UINT key)
	{
		return s_input_manager->IsConsumedKey(key);
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