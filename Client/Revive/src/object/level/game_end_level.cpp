#include <include/client_core.h>
#include <client/input/input.h>
#include "object/level/game_end_level.h"

namespace revive
{
	GameEndLevel::GameEndLevel(eGameResult game_result)
		:Level("game end level")
	{
		m_game_result = game_result;
	}
	bool GameEndLevel::Initialize()
	{
		switch (m_game_result)
		{
		case eGameResult::kWin:
			LOG_INFO("���� �¸�");
			break;
		case eGameResult::kDefeat:
			LOG_INFO("���� �й�");
			break;
		}

		return true;
	}
	void GameEndLevel::Shutdown()
	{
	}
	void GameEndLevel::Update(float delta_time)
	{
	}
	void GameEndLevel::ExecuteMessageFromServer(const SPtr<MessageEventInfo>& message)
	{
	}
}