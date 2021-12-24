#include "stdafx.h"
#include "client/core/application.h"
#include "client/core/timer.h"
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
		m_renderer->Shutdown();
		m_renderer = nullptr;

#if defined(_DEBUG)
		IDXGIDebug1* giDebug = nullptr;
		DXGIGetDebugInterface1(0, IID_PPV_ARGS(&giDebug));
		HRESULT result = giDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL);
		giDebug->Release();
#endif
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
	}

	void Application::Update(float delta_time)
	{
	}

	void Application::Render()
	{
		if (m_renderer->Render() == false)
		{
			LOG_ERROR("Rendering Error");
			SetAppState(eAppState::kDead);
		}
	}

	void Application::ChangeWindowSize(UINT width, UINT height)
	{
		m_window->width = width;
		m_window->height = height;
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
				app->ChangeWindowSize(LOWORD(lParam), HIWORD(lParam));
			}
			break;
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
			break;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MOUSEMOVE:
			break;
		case WM_DESTROY:
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		default:
			result = DefWindowProc(hWnd, message, wParam, lParam);
			break;
		}

		return result;
	}

}