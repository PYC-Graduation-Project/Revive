#include <include/client_core.h>
#include "revive_level_shared_info.h"

namespace revive
{
	ReviveLevelSharedInfo::ReviveLevelSharedInfo()
	{
		m_revive_game_option = CreateSPtr<ReviveGameOption>();
		m_map_loader = CreateSPtr<MapLoader>();
	}
}

