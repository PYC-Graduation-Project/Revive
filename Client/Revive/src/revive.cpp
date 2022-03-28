#include <include/client_fw.h>
#include <client/core/entry_point.h>
#include "object/level/game_play_level.h"

using namespace client_fw;

namespace revive
{
	class Revive : public client_fw::Application
	{
	public:
		Revive() : Application(L"Revive")
		{

		}

		bool Initialize() override
		{
			bool result = Application::Initialize();
			
			if (result)
			{
				LOG_INFO("Welcome to Revive Application");
				//Network::GetInst()->Init();
				//Network::GetInst()->CreateWorker();

				RegisterPressedEvent("Clip Cursor", std::vector{ EventKeyInfo{eKey::kF3, {eAdditionalKey::kControl}} },
					[]()->bool {Input::SetClipCursor(!Input::IsClipCursor()); return true;  });
				RegisterPressedEvent("Hide Cursor", std::vector{ EventKeyInfo{eKey::kF2, {eAdditionalKey::kControl}} },
					[]()->bool {Input::SetHideCursor(!Input::IsHideCursor()); return true;  });
				RegisterPressedEvent("open render rect level", { {eKey::k1} },
					[this]()->bool {OpenLevel(CreateSPtr<GamePlayLevel>());  return true; });

			}

			
			return result;
		}

		void Shutdown() override
		{
			Application::Shutdown();
			//Network::DestroyInst();
			LOG_INFO("Good Bye");
		}

		virtual ~Revive()
		{

		}
	};
}

client_fw::UPtr<client_fw::Application> client_fw::CreateApplication()
{
	return client_fw::CreateUPtr<revive::Revive>();
}