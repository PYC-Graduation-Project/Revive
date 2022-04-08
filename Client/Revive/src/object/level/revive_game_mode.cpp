#include <include/client_core.h>
#include <client/object/level/core/level.h>
#include <client/object/level/gamemode/game_mode_base.h>
#include <client/object/actor/player_controller.h>
#include <client/input/input.h>
#include "object/component/follow_camera.h"
#include "revive_game_mode.h"
#include "object/actor/revive_player.h"

namespace revive
{
	ReviveGameMode::ReviveGameMode()
	{
		//m_default_pawn = CreateSPtr<DefaultCharacter>();
		m_player = CreateSPtr<RevivePlayer>();
		m_player_controller = CreateSPtr<PlayerController>();
	}
	ReviveGameMode::~ReviveGameMode()
	{
		
	}
	bool ReviveGameMode::Initialize(const SPtr<Level>& level)
	{
		m_player_controller->Possess(m_player);
		m_player_controller->SetPlayerCamera(m_player->GetCameraComponent());
		level->SpawnActor(m_player);
		level->SpawnActor(m_player_controller);
		Input::SetInputMode(eInputMode::kGameOnly);
		return true;
	}
	void ReviveGameMode::Shutdown()
	{
		Input::SetInputMode(eInputMode::kUIOnly);
		m_default_pawn = nullptr;
		m_player_controller = nullptr;
		m_player = nullptr;
	}
}

