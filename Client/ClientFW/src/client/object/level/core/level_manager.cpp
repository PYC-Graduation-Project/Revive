#include "stdafx.h"
#include "client/object/level/core/level_manager.h"
#include "client/object/level/core/level.h"
#include "client/object/level/core/level_loader.h"

namespace client_fw
{
	void LevelManager::Shutdown()
	{
		if (m_ready_level != nullptr)
			m_ready_level->ShutdownLevel();
		if (m_cur_level != nullptr)
			m_cur_level->ShutdownLevel();
	}

	void LevelManager::Update(float delta_time)
	{
		if (m_ready_level != nullptr)
		{
			if (m_cur_level != nullptr)
			{
				m_cur_level->ShutdownLevel();
				m_cur_level = nullptr;
			}

			m_cur_level = std::move(m_ready_level);
			m_ready_level = nullptr;
		}

		if (m_cur_level != nullptr)
		{
			if (m_cur_level->GetLevelState() == eLevelState::kDead)
			{
				m_cur_level->ShutdownLevel();
				m_cur_level = nullptr;
			}
			else
			{
				m_cur_level->UpdateLevel(delta_time);
			}
		}
	}

	void LevelManager::OpenLevel(const SPtr<Level> new_level, UPtr<LevelLoader>&& level_loader)
	{
		if (m_ready_level != nullptr)
		{
			m_ready_level->ShutdownLevel();
			m_ready_level = nullptr;
		}

		m_ready_level = new_level;
		m_ready_level->InitializeLevel();
		if (level_loader != nullptr)
		{
			level_loader->LoadLevel(m_ready_level);
		}
	}

	void LevelManager::CloseLevel()
	{
		m_cur_level->SetLevelState(eLevelState::kDead);
	}
}
