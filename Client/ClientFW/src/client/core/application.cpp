#include "stdafx.h"
#include "client/core/application.h"
#include "client/core/window.h"
#include "client/core/timer.h"
#include "client/input/input.h"
#include "client/event/inputevent/input_event_system.h"
#include "client/object/level/core/level_manager.h"
#include "client/object/level/core/level_loader.h"
#include "client/object/level/core/level.h"
#include "client/renderer/core/renderer.h"

namespace client_fw
{
	Application* Application::s_instance = nullptr;


	Application::Application(std::wstring_view app_name)
		: m_app_state(eAppState::kActive)
		, m_app_name(app_name)
	{
		s_instance = this;

		m_window = CreateSPtr<Window>(1366, 768);
		m_timer = CreateUPtr<Timer>();
		m_input_event_system = CreateUPtr<InputEventSystem>(m_window);
		m_level_manager = CreateUPtr<LevelManager>();
		m_renderer = CreateUPtr<Renderer>(m_window);
	}

	Application::~Application()
	{
	}

	bool Application::Initialize()
	{
		bool result = InitializeWindow();

		if (result == false)
		{
			LOG_ERROR("Could not initialize window");
			return false;
		}

		result = m_timer->Initialize();
		if (result == false)
		{
			LOG_ERROR("Could not initialize timer");
			return false;
		}
		m_timer->OnFpsChanged([this](UINT fps) {ShowFpsToWindowTitle(fps); });

		result = m_renderer->Initialize();
		if (result == false)
		{
			LOG_ERROR("Could not initialize renderer");
			return false;
		}

		return true;
	}

	void Application::Shutdown()
	{
		m_level_manager->Shutdown();
		m_renderer->Shutdown();
	}

	void Application::Run()
	{
		MSG msg;

		while (GetAppState() != eAppState::kDead)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT) break;

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else if (GetAppState() == eAppState::kActive)
			{
				ProcessInput();
				m_timer->Update();
				Update(m_timer->GetDeltaTime());
				Render();
			}
		}
	}

	void Application::ProcessInput()
	{
		m_input_event_system->ExecuteEvent();
	}

	void Application::Update(float delta_time)
	{
		m_level_manager->Update(delta_time);
	}

	void Application::Render()
	{
		if (m_renderer->Render() == false)
		{
			LOG_ERROR("Rendering Error");
			SetAppState(eAppState::kDead);
		}
	}

	void Application::UpdateWindowSize()
	{
		UpdateWindowRect();
		m_window->width = m_window->rect.right - m_window->rect.left;
		m_window->height = m_window->rect.bottom - m_window->rect.top;
		m_window->mid_pos.x = m_window->width / 2;
		m_window->mid_pos.y = m_window->height / 2;
	}

	void Application::UpdateWindowRect()
	{
		GetWindowRect(m_window->hWnd, &m_window->rect);
	}

	void Application::RegisterPressedEvent(std::string_view name, std::vector<EventKeyInfo>&& keys, 
		const std::function<bool()>& func, bool consumption)
	{
		Input::RegisterPressedEvent(name, std::move(keys), func, consumption, eInputOwnerType::kApplication);
	}

	void Application::OpenLevel(const SPtr<Level>& level)
	{
		m_level_manager->OpenLevel(level, nullptr);
	}

	void Application::OpenLevel(const SPtr<Level>& level, UPtr<LevelLoader>&& level_loader)
	{
		m_level_manager->OpenLevel(level, std::move(level_loader));
	}

	void Application::CloseLevel()
	{
		m_level_manager->CloseLevel();
	}

	bool Application::InitializeWindow()
	{
		m_window->hInstance = GetModuleHandle(nullptr);
		if (m_window->hInstance == nullptr)
			return false;

		WNDCLASSEXW wcex;
		wcex.cbSize = sizeof(WNDCLASSEXW);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = m_window->hInstance;
		wcex.hIcon = LoadIcon(m_window->hInstance, IDI_WINLOGO);
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = m_app_name.c_str();
		wcex.hIconSm = wcex.hIcon;

		RegisterClassExW(&wcex);

		int posX = (GetSystemMetrics(SM_CXSCREEN) == m_window->width) ? 0 : (GetSystemMetrics(SM_CXSCREEN) - m_window->width) / 2;
		int posY = (GetSystemMetrics(SM_CYSCREEN) == m_window->height) ? 0 : (GetSystemMetrics(SM_CYSCREEN) - m_window->height) / 2;

		m_window->hWnd = CreateWindowEx(WS_EX_APPWINDOW, m_app_name.c_str(), m_app_name.c_str(),
			WS_OVERLAPPEDWINDOW, posX, posY, m_window->width, m_window->height, NULL, NULL, m_window->hInstance, NULL);

		if (m_window->hWnd == nullptr)
			return false;
		
		//SetWindowLong(m_window->hWnd, GWL_STYLE, 0);

#ifndef _DEBUG
		ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif 
		ShowWindow(m_window->hWnd, SW_SHOW);
		SetForegroundWindow(m_window->hWnd);
		SetFocus(m_window->hWnd);
		return true;
	}

	void Application::DestroyWindow()
	{
		::DestroyWindow(m_window->hWnd);
		UnregisterClassW(m_app_name.c_str(), m_window->hInstance);
	}

	void Application::ShowFpsToWindowTitle(UINT fps)
	{
		std::wstring title = m_app_name;
		title += L" (FPS : " + std::to_wstring(fps) + L")";
		SetWindowTextW(m_window->hWnd, title.c_str());
	}

	LRESULT CALLBACK Application::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		LRESULT result = NULL;

		const auto& app = Application::GetApplication();
		const auto& input_system = app->m_input_event_system;

		switch (message)
		{
		case WM_SIZE:
			if (wParam == SIZE_MINIMIZED)
			{
				app->SetAppState(client_fw::eAppState::kPaused);
			}
			else if (wParam == SIZE_MAXIMIZED || wParam == SIZE_RESTORED)
			{
				app->SetAppState(client_fw::eAppState::kActive);
				app->UpdateWindowSize();
			}
			break;
		case WM_MOVE:
			app->UpdateWindowRect();
			break;
		case WM_DESTROY:
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		default:
			result = DefWindowProc(hWnd, message, wParam, lParam);
			break;
		}

		input_system->ChangeInputState(message, wParam, lParam);

		return result;
	}

}