#pragma once
#include "client/object/core/base_object_manager.h"
#include <stack>

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
		void UpdateStaticActors(float delta_time);
		void UpdateDynamicActors(float delta_time);

	private:
		std::vector<SPtr<Actor>> m_ready_actors;
		std::vector<SPtr<Actor>> m_static_actors;
		std::vector<SPtr<Actor>> m_dynamic_actors;

	};
}


