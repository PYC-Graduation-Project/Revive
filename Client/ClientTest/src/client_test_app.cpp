#include <client_fw.h>
#include <client/core/entry_point.h>

using namespace client_fw;

namespace revive
{
	class ClientTestApp : public client_fw::Application
	{
	public:
		ClientTestApp() : Application(L"Client Test App")
		{
		}

		bool Initialize() override
		{
			bool result = Application::Initialize();

			if (result)
			{
				LOG_INFO("Welcome to Client Test App");
			}

			RegisterPressedEvent("Clip Cursor", std::vector{ EventKeyInfo{INPUT_KEY_F3, EAdditionalKey::kCtrl} },
				[]() {Input::SetClipCursor(!Input::IsClipCursor()); });
			RegisterPressedEvent("Hide Cursor", std::vector{ EventKeyInfo{INPUT_KEY_F2, EAdditionalKey::kCtrl} },
				[]() {Input::SetHideCursor(!Input::IsHideCursor()); });

			return result;
		}

		void Shutdown() override
		{
			Application::Shutdown();

			LOG_INFO("Good Bye");
		}

		void ProcessInput() override
		{
			Application::ProcessInput();
		}

		void Update(float delta_time) override
		{
			Application::Update(delta_time);
		}

		virtual ~ClientTestApp()
		{
		}
	};
}

client_fw::UPtr<client_fw::Application> client_fw::CreateApplication()
{
	return client_fw::CreateUPtr<revive::ClientTestApp>();
}