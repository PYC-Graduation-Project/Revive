#include <client_fw.h>
//#include <client/core/application.h>
//#include <client/core/log.h>
//#include <client/input/input.h>
#include <client/core/entry_point.h>
#include "object/level/client_test_level.h"

using namespace client_fw;

namespace client_test
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

			RegisterPressedEvent("Clip Cursor", std::vector{ EventKeyInfo{eKey::kF3, {eAdditionalKey::kControl}} },
				[]() {Input::SetClipCursor(!Input::IsClipCursor()); });
			RegisterPressedEvent("Hide Cursor", std::vector{ EventKeyInfo{eKey::kF2, {eAdditionalKey::kControl}} },
				[]() {Input::SetHideCursor(!Input::IsHideCursor()); });
			RegisterPressedEvent("Shutdown App", std::vector{ EventKeyInfo{eKey::kF4, {eAdditionalKey::kControl, eAdditionalKey::kShift}} },
				[this]() {SetAppState(eAppState::kDead); });

			auto level = CreateSPtr<ClientTestLevel>("Test1");
			OpenLevel(level);

			level = CreateSPtr<ClientTestLevel>("Test2");
			OpenLevel(level);

			return result;
		}

		void Shutdown() override
		{
			Application::Shutdown();

			LOG_INFO("Good Bye");
		}

		virtual ~ClientTestApp()
		{
		}
	};
}

client_fw::UPtr<client_fw::Application> client_fw::CreateApplication()
{
	return client_fw::CreateUPtr<client_test::ClientTestApp>();
}