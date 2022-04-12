#include <include/client_core.h>
#include "revive_game_mode.h"
#include "object/actor/revive_controller.h"
#include "object/actor/revive_player.h"

namespace revive
{
	ReviveGameMode::ReviveGameMode()
	{
		m_default_pawn = CreateSPtr<RevivePlayer>();
		m_player_controller = CreateSPtr<ReviveController>();
	}
	
}

