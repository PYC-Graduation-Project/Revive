#include "stdafx.h"
#include "client/input/input_manager.h"
#include "client/core/window.h"

namespace client_fw
{
	InputManager* Input::s_input_manager = nullptr;

	InputManager::InputManager(const WPtr<Window>& window)
		: m_window(window)
	{
        Input::s_input_manager = this;
	}

    InputManager::~InputManager()
    {
    }
     
    void InputManager::Update()
    {
        m_key_states[ToUnderlying(EKeyState::kBefore)] = 
            m_key_states[ToUnderlying(EKeyState::kCur)];

        m_key_states[ToUnderlying(EKeyState::kCur)][INPUT_MOUSE_XMOVE] =
            m_key_states[ToUnderlying(EKeyState::kCur)][INPUT_MOUSE_YMOVE] = false;

        if (m_is_hide_cursor)
        {
            const auto& window = m_window.lock();
            POINT pos = window->mid_pos;
            ClientToScreen(window->hWnd, &pos);
            SetCursorPos(pos.x, pos.y);
        }
        else
        {
            m_mouse_position[ToUnderlying(EMousePosState::kBefore)] =
                m_mouse_position[ToUnderlying(EMousePosState::kCur)];
        }

	}

    bool InputManager::IsKeyHoldDown(UINT key) const
    {
        return m_key_states[ToUnderlying(EKeyState::kCur)][key] && 
            m_key_states[ToUnderlying(EKeyState::kBefore)][key];
    }

    bool InputManager::IsKeyPressed(UINT key) const
    {
        return m_key_states[ToUnderlying(EKeyState::kCur)][key] &&
            !m_key_states[ToUnderlying(EKeyState::kBefore)][key];
    }

    bool InputManager::IsKeyReleased(UINT key) const
    {
        return !m_key_states[ToUnderlying(EKeyState::kCur)][key] &&
            m_key_states[ToUnderlying(EKeyState::kBefore)][key];
    }

    bool InputManager::IsNotKeyHoldDown(UINT key) const
    {
        return !m_key_states[ToUnderlying(EKeyState::kCur)][key] &&
            !m_key_states[ToUnderlying(EKeyState::kBefore)][key];
    }

    const IVec2& InputManager::GetMousePosition() const
    {
        return m_mouse_position[ToUnderlying(EMousePosState::kCur)];
    }

    const IVec2 InputManager::GetRelativeMoustPosition() const
    {
        return m_mouse_position[ToUnderlying(EMousePosState::kCur)] -
            m_mouse_position[ToUnderlying(EMousePosState::kBefore)];
    }

    void InputManager::SetHideCursor(bool hide)
    {
        if (hide)
        {
            m_mouse_position[ToUnderlying(EMousePosState::kLastShow)] = 
                m_mouse_position[ToUnderlying(EMousePosState::kCur)];

            POINT pos = m_window.lock()->mid_pos;
            m_mouse_position[ToUnderlying(EMousePosState::kBefore)] =
                IVec2(pos.x, pos.y);

            SetClipCursor(true);
        }
        else
        {
            m_mouse_position[ToUnderlying(EMousePosState::kCur)] =
                m_mouse_position[ToUnderlying(EMousePosState::kBefore)] =
                m_mouse_position[ToUnderlying(EMousePosState::kLastShow)];

            POINT pos = { 
                m_mouse_position[ToUnderlying(EMousePosState::kLastShow)].x,
                m_mouse_position[ToUnderlying(EMousePosState::kLastShow)].y 
            };
           
            ClientToScreen(m_window.lock()->hWnd, &pos);
            SetCursorPos(pos.x, pos.y);
        }

        m_is_hide_cursor = hide;
        ShowCursor(!hide);
    }

    void InputManager::SetClipCursor(bool clip)
    {
        if (m_is_hide_cursor == false)
        {
            m_is_clip_cursor = clip;
            if (clip)
            {
                ClipCursor(&m_window.lock()->rect);
            }
            else
            {
                ClipCursor(nullptr);
            }
        }
    }

    void InputManager::ChangeKeyState(UINT message, WPARAM key, LPARAM flags)
	{
		bool pressed = (message == WM_KEYDOWN || message == WM_SYSKEYDOWN);
		m_key_states[ToUnderlying(EKeyState::kCur)].set(key, pressed);
	}

	void InputManager::ChangeMouseState(int button, WPARAM wParam, int x, int y)
	{
        bool down = false;
        HWND hWnd = m_window.lock()->hWnd;  

        switch (button)
        {
        case WM_LBUTTONDOWN:
            SetCapture(hWnd);
            button = INPUT_MOUSE_LBUTTON;
            down = true;
            break;
        case WM_LBUTTONUP:
            ReleaseCapture();
            button = INPUT_MOUSE_LBUTTON;
            down = false;
            break;
        case WM_RBUTTONDOWN:
            SetCapture(hWnd);
            button = INPUT_MOUSE_RBUTTON;
            down = true;
            break;
        case WM_RBUTTONUP:
            ReleaseCapture();
            button = INPUT_MOUSE_RBUTTON;
            down = false;
            break;
        case WM_MBUTTONDOWN:
            SetCapture(hWnd);
            button = INPUT_MOUSE_MBUTTON;
            down = true;
            break;
        case WM_MBUTTONUP:
            ReleaseCapture();
            button = INPUT_MOUSE_MBUTTON;
            down = false;
            break;
        case WM_MOUSEMOVE:
            if (m_mouse_position[ToUnderlying(EMousePosState::kCur)].x != x)
            {
                m_mouse_position[ToUnderlying(EMousePosState::kCur)].x = x;
                m_key_states[ToUnderlying(EKeyState::kCur)][INPUT_MOUSE_XMOVE] = true;
            }
            if (m_mouse_position[ToUnderlying(EMousePosState::kCur)].y != y)
            {
                m_mouse_position[ToUnderlying(EMousePosState::kCur)].y = y;
                m_key_states[ToUnderlying(EKeyState::kCur)][INPUT_MOUSE_YMOVE] = true;
            }
            return;
        }
        
        m_key_states[ToUnderlying(EKeyState::kCur)][button] = down;
	}
}