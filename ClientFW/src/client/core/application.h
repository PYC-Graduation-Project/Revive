#pragma once

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

	class Application
	{
	public:
		Application(std::string_view app_name);
		virtual ~Application() = default;
		
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;

		virtual bool Initialize();
		void Shutdown();
		void Run();

	private:
		void ProcessInput();
		void Update();
		void Render();

	private:
		static Application* s_instance;
		eAppState m_app_state;
		std::string m_app_name;

	public:
		inline static Application* GetApplication() { return s_instance; }
		eAppState GetAppState() { return m_app_state; }
		void SetAppState(eAppState app_state) { m_app_state = app_state; }
	};

	UPtr<Application> CreateApplication();
}


