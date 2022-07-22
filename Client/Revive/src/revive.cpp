#include <include/client_core.h>
#include <include/client_fw.h>
#include <client/core/entry_point.h>

#include <client/object/level/sharedinfo/level_shared_info.h>
#include <client/asset/mesh/mesh_loader.h>
#include <client/asset/sound/core/sound_manager.h>
#include <client/asset/sound/sound.h>

#include "object/level/sharedinfo/revive_level_shared_info.h"
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

				LoadSound();

				OpenLevel(CreateSPtr<LobbyLevel>());
			}

			return result;
		}

		virtual SPtr<LevelSharedInfo> CreateLevelSharedInfo() const override
		{
			return CreateSPtr<ReviveLevelSharedInfo>();
		}

		virtual UPtr<MeshLoader> CreateMeshLoader() const override
		{
			return CreateUPtr<RevLoader>();
		}

		void LoadSound() //프로그램 실행 시 한번만 호출
		{
			client_fw::SoundManager::GetSoundManager().LoadSound("Contents/Sound/the_field_of_dreams.mp3", "lobby sound");
			client_fw::SoundManager::GetSoundManager().LoadSound("Contents/Sound/moon.ogg", "ingame sound");
			client_fw::SoundManager::GetSoundManager().LoadSound("Contents/Sound/ZombiesAreComing.ogg", "zombie"); //ㄱㅊ은데?
			client_fw::SoundManager::GetSoundManager().LoadSound("Contents/Sound/gun.wav", "fire");
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