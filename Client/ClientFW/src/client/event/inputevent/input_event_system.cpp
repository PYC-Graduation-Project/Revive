#include "stdafx.h"
#include "client/core/window.h"
#include "client/input/input.h"
#include "client/input/input_manager.h"
#include "client/event/inputevent//input_event_system.h"
#include "client/event/inputevent//input_event_manager.h"


namespace client_fw
{
	InputEventSystem* Input::s_input_event_system = nullptr;

	InputEventSystem::InputEventSystem(const WPtr<Window>& window)
	{
		Input::s_input_event_system = this;
		m_input_manager = CreateUPtr<InputManager>(window);
		m_input_event_manager = CreateUPtr<InputEventManager>();
	}

	InputEventSystem::~InputEventSystem()
	{
	}

	void InputEventSystem::ExecuteEvent()
	{
		m_input_event_manager->ExecuteEvent();
		m_input_manager->Update();
	}

	void InputEventSystem::ChangeInputState(UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
			m_input_manager->ChangeKeyState(message, wParam, lParam);
			break;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MOUSEMOVE:
			m_input_manager->ChangeMouseState(message, wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
		}
	}
}
