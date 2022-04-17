#include <include/client_fw.h>
#include <client/core/entry_point.h>
#include <client/asset/mesh/mesh_loader.h>
#include "object/level/event_test_level.h"
#include "object/level/physics_test_level.h"
#include "object/level/message_test_level.h"

#include"server/network.h"
#include"server/revive_send_manager.h"
#include"server/revive_packet_manager.h"

#include "object/level/frame_test_level.h"
#include "object/level/light_test_level.h"


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
			Network::GetInst()->Init(CreateUPtr<RevivePacketManager>(), CreateUPtr<ReviveSendManager>());
			Network::GetInst()->CreateWorker();
			RegisterPressedEvent("Input Mode Game Only", std::vector{ EventKeyInfo{eKey::k1, {eAdditionalKey::kControl}} },
				[]()->bool { Input::SetInputMode(eInputMode::kGameOnly); return true;  });
			RegisterPressedEvent("Input Mode Game And UI", std::vector{ EventKeyInfo{eKey::k2, {eAdditionalKey::kControl}} },
				[]()->bool { Input::SetInputMode(eInputMode::kUIAndGame); return true;  });
			RegisterPressedEvent("Input Mode UI Only", std::vector{ EventKeyInfo{eKey::k3, {eAdditionalKey::kControl}} },
				[]()->bool { Input::SetInputMode(eInputMode::kUIOnly); return true;  });

			RegisterPressedEvent("open event test level", { {eKey::k1} },
				[this]()->bool {OpenLevel(CreateSPtr<EventTestLevel>()); return true; });

			RegisterPressedEvent("open physics test level", { {eKey::k2} },
				[this]()->bool {OpenLevel(CreateSPtr<PhysicsTestLevel>()); return true; });

			RegisterPressedEvent("open message test level", { {eKey::k3} },
				[this]()->bool {OpenLevel(CreateSPtr<MessageTestLevel>()); return true; });

			RegisterPressedEvent("open frame test level", { {eKey::k4} },
				[this]()->bool {OpenLevel(CreateSPtr<FrameTestLevel>()); return true; });

			RegisterPressedEvent("open light test level", { {eKey::k5} },
				[this]()->bool {OpenLevel(CreateSPtr<LightTestLevel>()); return true; });

			return result;
		}

		UPtr<MeshLoader> CreateMeshLoader() const override
		{
			return CreateUPtr<RevLoader>();
		}

		void Shutdown() override
		{
			Network::GetInst()->DestroyWorker();
			Network::DestroyInst();
			Application::Shutdown();
			
		}
	};
}

client_fw::UPtr<client_fw::Application> client_fw::CreateApplication()
{
	return client_fw::CreateUPtr<event_test::RenderTestApp>();
}