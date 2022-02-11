#include <include/client_fw.h>
#include <client/core/entry_point.h>
#include "object/level/player_test_level.h"
using namespace client_fw;

namespace animation_test
{
	class AnimationTestApp : public client_fw::Application
	{
	public:
		AnimationTestApp() : Application(L"Animation Test App")
		{

		}
		
		bool Initialize() override
		{
			bool result = Application::Initialize();
			RegisterPressedEvent("Clip Cursor", std::vector{ EventKeyInfo{eKey::kF3, {eAdditionalKey::kControl}} },
				[]()->bool {Input::SetClipCursor(!Input::IsClipCursor()); return true;  });
			RegisterPressedEvent("Hide Cursor", std::vector{ EventKeyInfo{eKey::kF2, {eAdditionalKey::kControl}} },
				[]()->bool {Input::SetHideCursor(!Input::IsHideCursor()); return true;  });

			RegisterPressedEvent("open player test level", { {eKey::k2} },
				[this]()->bool {OpenLevel(CreateSPtr<PlayerTestLevel>()); return true; });
			return result;
		}
		void InitializeAssetManager() override
		{
			client_fw::Application::m_asset_manager->Initialize(CreateUPtr<RevLoader>(), CreateUPtr<MaterialLoader>());
		}
		void Shutdown() override
		{
			Application::Shutdown();
		}
	};
}


client_fw::UPtr<client_fw::Application> client_fw::CreateApplication()
{
	return client_fw::CreateUPtr<animation_test::AnimationTestApp>();
}