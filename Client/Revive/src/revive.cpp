#include <include/client_core.h>
#include <include/client_fw.h>
#include <client/core/entry_point.h>

#include <client/asset/mesh/mesh_loader.h>
#include "object/level/game_play_level.h"
#include "object/level/lobby_level.h"
#include"server/network.h"
#include"revive_server/send/revive_send_manager.h"
#include"revive_server/packet/revive_packet_manager.h"
using namespace client_fw;


#include"server/send_manager.h"
#include"server/packet_manager.h"

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
				Network::GetInst()->Init(CreateUPtr<RevivePacketManager>(), CreateUPtr<ReviveSendManager>());
				Network::GetInst()->CreateWorker();

				RegisterPressedEvent("Clip Cursor", std::vector{ EventKeyInfo{eKey::kF3, {eAdditionalKey::kControl}} },
					[]()->bool {Input::SetClipCursor(!Input::IsClipCursor()); return true;  });
				RegisterPressedEvent("Hide Cursor", std::vector{ EventKeyInfo{eKey::kF2, {eAdditionalKey::kControl}} },
					[]()->bool {Input::SetHideCursor(!Input::IsHideCursor()); return true;  });
				
				RegisterPressedEvent("open lobby level", { {eKey::k1} },
					[this]()->bool {OpenLevel(CreateSPtr<LobbyLevel>());  return true; });
				RegisterPressedEvent("open game play level", { {eKey::k2} },
					[this]()->bool { OpenLevel(CreateSPtr<GamePlayLevel>()); 
			      return true; });
			}

			return result;
		}

		UPtr<MeshLoader> CreateMeshLoader() const override
		{
			return CreateUPtr<RevLoader>();
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