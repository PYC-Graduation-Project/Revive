#pragma once
#include "client/object/core/base_object.h"

namespace client_fw
{
	class Actor;
	class ActorManager;
	struct EventKeyInfo;
	class MeshOctree;

	enum class eLevelState
	{
		kInGame, kPaused, kDead
	};

	class Level : public IBaseObject
	{
	public:
		Level(const std::string& name = "level");
		virtual ~Level();

		bool InitializeLevel();
		virtual bool Initialize() override { return true; }

		void ShutdownLevel();
		virtual void Shutdown() override {}

		void UpdateLevel(float delta_time);
		virtual void Update(float delta_time) override {}

		void SpawnActor(const SPtr<Actor>& actor);

	protected:
		void RegisterPressedEvent(const std::string& name, std::vector<EventKeyInfo>&& keys,
			const std::function<bool()>& func, bool consumption = true);
		void RegisterReleasedEvent(const std::string& name, std::vector<EventKeyInfo>&& keys,
			const std::function<bool()>& func, bool consumption = true);

	private:
		void RegisterInputEvent(const std::string& name);

	protected:
		std::string m_name;
		eLevelState m_level_state;

	private:
		std::vector<std::string> m_registered_input_event;
		UPtr<ActorManager> m_actor_manager;
		bool m_is_runtime_level;

	public:
		const std::string& GetName() const { return m_name; }
		eLevelState GetLevelState() const { return m_level_state; }
		void SetLevelState(eLevelState level_state) { m_level_state = level_state; }
		void SetRuntime() { m_is_runtime_level = true; }
		bool IsRuntime() const { return m_is_runtime_level; }

		virtual SPtr<MeshOctree> CreateMeshOctree() const;
	};
}

