#include <include/client_core.h>
#include <client/input/input.h>
#include <client/object/actor/default_pawn.h>
#include <client/object/actor/player_controller.h>
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
		m_player_controller = CreateSPtr<PlayerController>();

		m_player_controller->Possess(m_player);

		SpawnActor(m_player);
		SpawnActor(m_player_controller);


		return true;
	}

	void PlayerTestLevel::Shutdown()
	{
	}

	void PlayerTestLevel::Update(float delta_time)
	{
		LOG_TRACE("controller : {0}", m_player_controller->GetForward());
		LOG_TRACE("player : {0}", m_player->GetForward());

	}
}
