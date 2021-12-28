#include "stdafx.h"
#include "client/input/input_manager.h"

namespace client_fw
{
	InputManager* Input::s_input_manager = nullptr;

	InputManager::InputManager(HWND hWnd)
		: m_hWnd(hWnd)
	{
        Input::s_input_manager = this;
	}

    InputManager::~InputManager()
    {
    }

	void InputManager::Update()
	{
		m_key_states[1] = m_key_states[0];
        m_mouse_position[1] = m_mouse_position[0];
	}

    void InputManager::SetHideCursor(bool hide)
    {
        if (hide) m_mouse_position[1] = m_mouse_position[0];
        m_is_hide_cursor = hide;
        ShowCursor(!hide);
    }

    void InputManager::ChangeKeyState(UINT message, WPARAM key, LPARAM flags)
	{
		bool pressed = (message == WM_KEYDOWN || message == WM_SYSKEYDOWN);
		m_key_states[0].set(key, pressed);
	}

	void InputManager::ChangeMouseState(int button, WPARAM wParam, int x, int y)
	{
        bool down = false;
        switch (button)
        {
        case WM_LBUTTONDOWN:
            SetCapture(m_hWnd);
            button = INPUT_MOUSE_LBUTTON;
            down = true;
            break;
        case WM_LBUTTONUP:
            ReleaseCapture();
            button = INPUT_MOUSE_LBUTTON;
            down = false;
            break;
        case WM_RBUTTONDOWN:
            SetCapture(m_hWnd);
            button = INPUT_MOUSE_RBUTTON;
            down = true;
            break;
        case WM_RBUTTONUP:
            ReleaseCapture();
            button = INPUT_MOUSE_RBUTTON;
            down = false;
            break;
        case WM_MBUTTONDOWN:
            SetCapture(m_hWnd);
            button = INPUT_MOUSE_MBUTTON;
            down = true;
            break;
        case WM_MBUTTONUP:
            ReleaseCapture();
            button = INPUT_MOUSE_MBUTTON;
            down = false;
            break;
        case WM_MOUSEMOVE:
            if (m_mouse_position[0].first != x)
            {
                m_mouse_position[0].first = x;
                button = INPUT_MOUSE_XMOVE;
                down = true;
            }
            if (m_mouse_position[0].second != y)
            {
                m_mouse_position[0].second = y;
                button = INPUT_MOUSE_YMOVE;
                down = true;
            }
            break;
        }
        
        m_key_states[0][button] = down;
	}
}