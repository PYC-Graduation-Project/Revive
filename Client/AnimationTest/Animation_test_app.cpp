#include <include/client_core.h>
#include <include/client_fw.h>
#include <client/core/entry_point.h>
#include <client/asset/mesh/mesh_loader.h>
#include <client/asset/material/material_loader.h>
#include <client/asset/core/asset_manager.h>
#include "object/level/player_test_level.h"
using namespace client_fw;

namespace anim_test
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
			m_asset_manager->Initialize(CreateUPtr<RevLoader>(), CreateUPtr<MaterialLoader>());
		}
		void Shutdown() override
		{
			Application::Shutdown();
		}
	};
}


client_fw::UPtr<client_fw::Application> client_fw::CreateApplication()
{
	return client_fw::CreateUPtr<anim_test::AnimationTestApp>();
}