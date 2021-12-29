#pragma once

namespace client_fw
{
#define MAX_KEYS		256

	struct Window;

	class InputManager final
	{
	private:
		friend class Application;
		friend class Input; 

		enum class EKeyState
		{
			kCur, kBefore,
		};

		enum class EMousePosState
		{
			kCur, kBefore, kLastShow,
		};

	public:
		InputManager(const WPtr<Window>& window);
		~InputManager();

		InputManager(const InputManager&) = delete;
		InputManager& operator=(const InputManager&) = delete;
		
	private:
		void Update();

	private:
		bool IsKeyHoldDown(UINT key) const;
		bool IsKeyPressed(UINT key) const;
		bool IsKeyReleased(UINT key) const;
		bool IsNotKeyHoldDown(UINT key) const;
		const IVec2& GetMousePosition() const;
		const IVec2 GetRelativeMoustPosition() const;

		void SetHideCursor(bool hide);
		bool IsHideCursor() const { return m_is_hide_cursor; }

		void SetClipCursor(bool clip);
		bool IsClipCursor() const { return m_is_clip_cursor; }

	private:
		void ChangeKeyState(UINT message, WPARAM key, LPARAM flags);
		void ChangeMouseState(int button, WPARAM wParam, int x, int y);

	private:
		WPtr<Window> m_window;
		std::array<std::bitset<MAX_KEYS>, 2> m_key_states;
		std::array<IVec2, 3> m_mouse_position; // For the index, refer to EMousePosState

		bool m_is_hide_cursor = false;
		bool m_is_clip_cursor = false;
	};
}



