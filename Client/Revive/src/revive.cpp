#include <include/client_core.h>
#include <include/client_fw.h>
#include <client/core/entry_point.h>

#include <client/asset/mesh/mesh_loader.h>

#include "object/level/game_play_level.h"
#include "object/level/lobby_level.h"

#include "server/network.h"
#include "revive_server/send/revive_send_manager.h"
#include "revive_server/packet/revive_packet_manager.h"
#include "server/send_manager.h"
#include "server/packet_manager.h"

namespace revive
{
	using namespace client_fw;

	class Revive : public Application
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
				Network::GetInst()->Init(CreateUPtr<RevivePacketManager>(), CreateUPtr<ReviveSendManager>());
				Network::GetInst()->CreateWorker();

				OpenLevel(CreateSPtr<LobbyLevel>());

			/*	RegisterPressedEvent("open game play level", { {eKey::k2} },
					[this]()->bool {  return true; });*/
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