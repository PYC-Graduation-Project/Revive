#include "stdafx.h"
#include "client/input/input_manager.h"

namespace client_fw
{
	inline bool Input::IsKeyHoldDown(UINT key)
	{
		return s_input_manager->IsKeyHoldDown(key);
	}

	inline bool Input::IsKeyPressed(UINT key)
	{
		return s_input_manager->IsKeyPressed(key);
	}

	inline bool Input::IsKeyReleased(UINT key)
	{
		return s_input_manager->IsKeyReleased(key);
	}

	inline bool Input::IsNotKeyHoldDown(UINT key)
	{
		return s_input_manager->IsNotKeyHoldDown(key);
	}

	inline void Input::SetHideCursor(bool hide)
	{
		if (hide != IsHideCursor())
			s_input_manager->SetHideCursor(hide);
	}

	inline bool Input::IsHideCursor()
	{
		return s_input_manager->IsHideCursor();
	}
}