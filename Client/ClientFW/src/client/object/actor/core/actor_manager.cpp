#include "stdafx.h"
#include "client/object/actor/core/actor_manager.h"
#include "client/object/actor/core/actor.h"

namespace client_fw
{
	void ActorManager::Shutdown()
	{
		for (const auto& actor : m_ready_actors)
			actor->ShutdownActor();
		for (const auto& actor : m_static_actors)
			actor->ShutdownActor();
		for (const auto& actor : m_movable_actors)
			actor->ShutdownActor();
	}

	void ActorManager::Update(float delta_time)
	{
		for (auto& actor : m_ready_actors)
		{
			actor->InitializeActor();
			switch (actor->GetMobilityState())
			{
			case eMobilityState::kStatic:
				m_static_actors.emplace_back(std::move(actor));
				break;
			case eMobilityState::kMovable:
				m_movable_actors.emplace_back(std::move(actor));
				break;
			}
		}
		m_ready_actors.clear();

		std::vector<SPtr<Actor>> static_dead_actors;
		std::vector<SPtr<Actor>> movable_dead_actors;

		UpdateActors(m_static_actors, static_dead_actors, delta_time);
		UpdateActors(m_movable_actors, movable_dead_actors, delta_time);

		UnregisterActor(m_static_actors, std::move(static_dead_actors));
		UnregisterActor(m_movable_actors, std::move(movable_dead_actors));
	}

	void ActorManager::UpdateActors(const std::vector<SPtr<Actor>>& actors, 
		std::vector<SPtr<Actor>>& dead_actors, float delta_time)
	{
		for (const auto& actor : actors)
		{
			switch (actor->GetActorState())
			{
			case eActorState::kActive:
				actor->UpdateActor(delta_time);
				break;
			case eActorState::kPaused:
				break;
			case eActorState::kDead:
				dead_actors.push_back(actor);
				break;
			}
		}
	}

	void ActorManager::RegisterActor(const SPtr<Actor>& actor)
	{
		m_ready_actors.push_back(actor);
	}

	void ActorManager::UnregisterActor(std::vector<SPtr<Actor>>& actors,
		std::vector<SPtr<Actor>>&& dead_actors)
	{
		for (auto& dead_actor : dead_actors)
		{
			auto iter = std::find_if(actors.begin(), actors.end(),
				[&dead_actor](const SPtr<Actor>& actor)
				{ return actor == dead_actor; });
			if (iter != actors.end())
			{
				dead_actor->ShutdownActor();
				actors.erase(iter);
			}
		}
	}


}
