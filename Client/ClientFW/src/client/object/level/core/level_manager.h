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
		void UpdateWorldMatrix();

		void OpenLevel(const SPtr<Level> new_level, UPtr<LevelLoader>&& level_loader);
		void CloseLevel() const;

		void SpawnActor(const SPtr<Actor>& actor) const;

	private:
		static LevelManager* s_instance;

		SPtr<Level> m_ready_level;
		SPtr<Level> m_cur_level;
		SPtr<Level> m_dead_level;
		UPtr<OctreeManager> m_octree_manager;
		std::vector<std::function<void()>> m_level_close_events;

	public:
		inline static LevelManager& GetLevelManager() { return *s_instance; }
		const SPtr<Level>& GetCurrentLevel() const { return m_cur_level; }
		void AddLevelCloseEvent(const std::function<void()>& function);
	};
}


