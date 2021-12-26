#pragma once

namespace client_fw
{
#define MAX_KEYS		256

	//Math 관련 기능을 추가하기 전에 임시로 사용할 기능
	using Pos2 = std::pair<int, int>;

	class InputManager final
	{
	private:
		friend class Input;
		friend class Application;

	private:
		InputManager(HWND hWnd);

		InputManager(const InputManager&) = delete;
		InputManager& operator=(const InputManager&) = delete;
		
		void Update();

	private:
		bool IsKeyHoldDown(UINT key) const { return m_key_states[0][key] && m_key_states[1][key]; }
		bool IsKeyPressed(UINT key) const { return m_key_states[0][key] && !m_key_states[1][key]; }
		bool IsKeyReleased(UINT key) const { return !m_key_states[0][key] && m_key_states[1][key]; }
		bool IsNotKeyHoldDown(UINT key) const { return !m_key_states[0][key] && m_key_states[1][key]; }
		const Pos2& GetMousePosition() const { return m_mouse_position[0]; }
		//const Pos2 GetRelativeMoustPosition() const { return m_mouse_position[0] - m_mouse_position[1]; }

		void SetHideCursor(bool hide);
		bool IsHideCursor() const { return m_is_hide_cursor; }

	private:
		void ChangeKeyState(UINT message, WPARAM key, LPARAM flags);
		void ChangeMouseState(int button, WPARAM wParam, int x, int y);

	private:
		HWND m_hWnd;
		std::array<std::bitset<MAX_KEYS>, 2> m_key_states;
		std::array<Pos2, 2> m_mouse_position;

		bool m_is_hide_cursor = false;
	};
}



