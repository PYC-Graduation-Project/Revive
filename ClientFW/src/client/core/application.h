#pragma once

#include <Windows.h>
#include <string>
#include "client/core/core.h"

namespace client_fw
{
	enum class eAppState
	{
		kActive,
		kPaused,
		kDead
	};

	struct Window
	{
		UINT width, height;
		HWND hWnd;
		HINSTANCE hInstance;

		Window(UINT width = GetSystemMetrics(SM_CXSCREEN), UINT height = GetSystemMetrics(SM_CYSCREEN))
			: width(width), height(height)
			, hWnd(nullptr), hInstance(nullptr)
		{}
	};

	class Timer;

	class Application
	{
	public:
		Application(std::wstring_view app_name);
		virtual ~Application();
		
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;

		virtual bool Initialize();
		virtual void Shutdown();
		void Run();

	private:
		void ProcessInput();
		void Update(float delta_time);
		void Render();

	public:
		void ChangeWindowSize(UINT width, UINT height);

	private:
		virtual bool InitializeWindow();
		void DestroyWindow();
		void ShowFpsToWindowTitle(UINT fps);

	protected:
		static Application* s_instance;
		eAppState m_app_state;
		std::wstring m_app_name;

		SPtr<Window> m_window;
		UPtr<Timer> m_timer;

	public:
		inline static Application* GetApplication() { return s_instance; }
		eAppState GetAppState() { return m_app_state; }
		void SetAppState(eAppState app_state) { m_app_state = app_state; }

	public:
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	};

	UPtr<Application> CreateApplication();
}


