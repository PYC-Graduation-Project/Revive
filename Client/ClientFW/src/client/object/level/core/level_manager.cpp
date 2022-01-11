#include "stdafx.h"
#include "client/object/level/core/level_manager.h"
#include "client/object/level/core/level.h"
#include "client/object/level/core/level_loader.h"
#include "client/object/actor/core/actor.h"

namespace client_fw
{
	LevelManager* LevelManager::s_instance = nullptr;

	LevelManager::LevelManager()
	{
		s_instance = this;
	}

	void LevelManager::Shutdown()
	{
		if (m_ready_level != nullptr)
			m_ready_level->ShutdownLevel();
		if (m_cur_level != nullptr)
			m_cur_level->ShutdownLevel();
		s_instance = nullptr;
	}

	void LevelManager::Update(float delta_time)
	{
		if (m_cur_level != nullptr && m_cur_level->GetLevelState() == eLevelState::kDead)
		{
			m_cur_level->ShutdownLevel();
			m_cur_level = nullptr;
		}

		if (m_ready_level != nullptr)
		{
			if (m_ready_level->InitializeLevel())
			{
				m_cur_level = std::move(m_ready_level);
				m_cur_level->SetRuntime();
				m_ready_level = nullptr;
			}
			else
			{
				LOG_WARN("Could not initailize level : {0}", m_ready_level->GetName());
				m_ready_level->ShutdownLevel();
			}
		}

		if (m_cur_level != nullptr)
		{
			m_cur_level->UpdateLevel(delta_time);
		}
	}

	void LevelManager::OpenLevel(const SPtr<Level> new_level, UPtr<LevelLoader>&& level_loader)
	{
		if (m_ready_level != nullptr)
		{
			m_ready_level->ShutdownLevel();
			m_ready_level = nullptr;
		}
		CloseLevel();
		m_ready_level = new_level;
	/*	if (level_loader != nullptr)
		{
			level_loader->LoadLevel(m_ready_level);
		}*/
	}

	void LevelManager::CloseLevel()
	{
		if(m_cur_level != nullptr)
			m_cur_level->SetLevelState(eLevelState::kDead);
	}

	void LevelManager::SpawnActor(const SPtr<Actor>& actor)
	{
		if (m_cur_level != nullptr)
			m_cur_level->SpawnActor(actor);
		else
			LOG_WARN("Could not create \"{0}\" : current level is nullptr", actor->GetName());
	}
}
