#include <include/client_core.h>
#include <client/object/level/core/level.h>
#include <client/input/input.h>
#include "object/gamemode/revive_game_mode.h"
#include "object/actor/character/revive_controller.h"
#include "object/actor/character/revive_player.h"

namespace revive
{
	ReviveGameMode::ReviveGameMode()
	{
		m_default_pawn = CreateSPtr<RevivePlayer>();
		m_player_controller = CreateSPtr<ReviveController>();
	}
	bool ReviveGameMode::Initialize(const SPtr<Level>& level)
	{
		m_player_controller->Possess(m_default_pawn);
		level->SpawnActor(m_default_pawn);
		level->SpawnActor(m_player_controller);
		Input::SetInputMode(eInputMode::kGameOnly);
		return true;
	}
}

