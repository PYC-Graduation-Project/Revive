#include "stdafx.h"
#include "client/object/level/core/level.h"

namespace client_fw
{
	Level::Level(std::string_view name)
		: m_name(name), m_level_state(eLevelState::kInGame)
	{
	}

	void Level::InitializeLevel()
	{
		Initialize();
	}

	void Level::ShutdownLevel()
	{
		Shutdown();
	}

	void Level::UpdateLevel(float delta_time)
	{
		Update(delta_time);
	}
}