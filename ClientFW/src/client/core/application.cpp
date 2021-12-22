#include "stdafx.h"
#include "client/core/application.h"

namespace client_fw
{
	Application* Application::s_instance = nullptr;


	Application::Application(std::string_view app_name)
		: m_app_state(eAppState::kActive)
		, m_app_name(app_name)
	{
		s_instance = this;
	}

	bool Application::Initialize()
	{
		std::cout << "Hello World" << std::endl;
		return true;
	}

	void Application::Shutdown()
	{
		std::cout << "Good Bye" << std::endl;
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
				Update();
				Render();
			}
		}
	}

	void Application::ProcessInput()
	{
	}

	void Application::Update()
	{
	}

	void Application::Render()
	{
	}

}