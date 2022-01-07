#pragma once

namespace client_fw
{
	enum class eAppState
	{
		kActive,
		kPaused,
		kDead
	};

	struct Window;
	class Timer;
	class Renderer;

	class InputEventSystem;
	struct EventKeyInfo;
	class LevelManager;
	class LevelLoader;
	class Level;

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

	private:
		virtual bool InitializeWindow();
		void DestroyWindow();
		void ShowFpsToWindowTitle(UINT fps);
		void UpdateWindowSize();
		void UpdateWindowRect();

	protected:
		void RegisterPressedEvent(std::string_view name, std::vector<EventKeyInfo>&& keys,
			const std::function<bool()>& func, bool consumption = true);

		void OpenLevel(const SPtr<Level>& level);
		void OpenLevel(const SPtr<Level>& level, UPtr<LevelLoader>&& level_loader);
		void CloseLevel();

	protected:
		static Application* s_instance;
		eAppState m_app_state;
		std::wstring m_app_name;

		SPtr<Window> m_window;

	private:
		UPtr<Timer> m_timer;
		UPtr<InputEventSystem> m_input_event_system;
		UPtr<LevelManager> m_level_manager;
		UPtr<Renderer> m_renderer;

	public:
		inline static Application* GetApplication() { return s_instance; }
		eAppState GetAppState() { return m_app_state; }
		void SetAppState(eAppState app_state) { m_app_state = app_state; }

	public:
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	};

	UPtr<Application> CreateApplication();
}


