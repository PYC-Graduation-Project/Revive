#pragma once
#include "client/object/core/base_object_manager.h"

namespace client_fw
{
	class Level;
	class LevelLoader;

	class Actor;
	class OctreeManager;

	class LevelManager final: public IBaseObjectManager
	{
	public:
		LevelManager();
		virtual ~LevelManager();

		LevelManager(const LevelManager&) = delete;
		LevelManager& operator=(const LevelManager&) = delete;

		virtual void Shutdown() override;
		virtual void Update(float delta_time) override;

		void OpenLevel(const SPtr<Level> new_level, UPtr<LevelLoader>&& level_loader);
		void CloseLevel();

		void SpawnActor(const SPtr<Actor>& actor);

	private:
		static LevelManager* s_instance;

		SPtr<Level> m_ready_level;
		SPtr<Level> m_cur_level;
		UPtr<OctreeManager> m_octree_manager;

	public:
		inline static LevelManager& GetLevelManager() { return *s_instance; }
	};
}


