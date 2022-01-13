#include <include/client_core.h>
#include <client/input/input.h>
#include <client/object/actor/default_pawn.h>
#include "object/level/player_test_level.h"

namespace render_test
{
	PlayerTestLevel::PlayerTestLevel()
		: Level("player test level")
	{
	}

	bool PlayerTestLevel::Initialize()
	{
		m_player = CreateSPtr<DefaultPawn>();

		SpawnActor(m_player);

		return true;
	}

	void PlayerTestLevel::Shutdown()
	{
	}

	void PlayerTestLevel::Update(float delta_time)
	{
	}
}
