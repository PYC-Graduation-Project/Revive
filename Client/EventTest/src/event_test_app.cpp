#include <include/client_fw.h>
#include <client/core/entry_point.h>
#include "object/level/event_test_level.h"

using namespace client_fw;

namespace event_test
{
	class RenderTestApp : public client_fw::Application
	{
	public:
		RenderTestApp() : Application(L"Render Test App")
		{
		}

		bool Initialize() override
		{
			bool result = Application::Initialize();

			RegisterPressedEvent("Clip Cursor", std::vector{ EventKeyInfo{eKey::kF3, {eAdditionalKey::kControl}} },
				[]()->bool {Input::SetClipCursor(!Input::IsClipCursor()); return true;  });
			RegisterPressedEvent("Hide Cursor", std::vector{ EventKeyInfo{eKey::kF2, {eAdditionalKey::kControl}} },
				[]()->bool {Input::SetHideCursor(!Input::IsHideCursor()); return true;  });

			RegisterPressedEvent("open event test level", { {eKey::k1} },
				[this]()->bool {OpenLevel(CreateSPtr<EventTestLevel>()); return true; });

			return result;
		}

		void Shutdown() override
		{
			Application::Shutdown();
		}
	};
}

client_fw::UPtr<client_fw::Application> client_fw::CreateApplication()
{
	return client_fw::CreateUPtr<event_test::RenderTestApp>();
}