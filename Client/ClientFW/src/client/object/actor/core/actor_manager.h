#pragma once
#include "client/object/core/base_object_manager.h"

namespace client_fw
{
	class Actor;

	class ActorManager final : public IBaseObjectManager
	{
	public:
		ActorManager() = default;
		virtual ~ActorManager() = default;

		ActorManager(const ActorManager&) = delete;
		ActorManager& operator=(const ActorManager&) = delete;

		virtual void Shutdown() override;
		virtual void Update(float delta_time) override;

		void RegisterActor(const SPtr<Actor>& actor);

	private:
		void UpdateActors(const std::vector<SPtr<Actor>>& actors, 
			std::vector<SPtr<Actor>>& dead_actors, float delta_time);

		void UnregisterActor(std::vector<SPtr<Actor>>& actors,
			std::vector<SPtr<Actor>>&& dead_actors);

	private:
		std::vector<SPtr<Actor>> m_ready_actors;
		std::vector<SPtr<Actor>> m_static_actors;
		std::vector<SPtr<Actor>> m_movable_actors;

	};
}


