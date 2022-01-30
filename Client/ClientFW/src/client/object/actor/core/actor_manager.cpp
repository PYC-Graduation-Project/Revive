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
		for (const auto& actor : m_dynamic_actors)
			actor->ShutdownActor();
	}

	void ActorManager::Update(float delta_time)
	{
		for (auto& actor : m_ready_actors)
		{
			UINT64 index = ++m_actor_names[actor->GetName()];
			actor->SetName(actor->GetName() + std::to_string(index));

			if (actor->InitializeActor())
			{
				switch (actor->GetMobilityState())
				{
				case eMobilityState::kStatic:
					m_static_actors.emplace_back(std::move(actor));
					break;
				case eMobilityState::kDestructable:
				case eMobilityState::kMovable:
					m_dynamic_actors.emplace_back(std::move(actor));
					break;
				}
			}
			else
			{
				LOG_ERROR("Could not initialize actor : {0}", actor->GetName());
				actor->ShutdownActor();
			}
		}
		m_ready_actors.clear();

		UpdateStaticActors(delta_time);
		UpdateDynamicActors(delta_time);
	}

	void ActorManager::UpdateStaticActors(float delta_time)
	{
		for (const auto& actor : m_static_actors)
		{
			switch (actor->GetActorState())
			{
			case eActorState::kActive:
				actor->UpdateActor(delta_time);
				break;
			case eActorState::kPaused:
				break;
			case eActorState::kDead:
				LOG_WARN("Static actor[{0}] cannot be deleted at runtime", actor->GetName());
				actor->SetActorState(eActorState::kActive);
				break;
			}
		}
	}

	void ActorManager::UpdateDynamicActors(float delta_time)
	{
		int count = 0;

		for (auto actor = m_dynamic_actors.rbegin(); actor != m_dynamic_actors.rend(); ++actor)
		{
			switch ((*actor)->GetActorState())
			{
			case eActorState::kActive:
				(*actor)->UpdateActor(delta_time);
				break;
			case eActorState::kPaused:
				break;
			case eActorState::kDead:
				(*actor)->ShutdownActor();
				std::iter_swap(actor, m_dynamic_actors.rbegin() + count);
				++count;
				break;
			}
		}

		while (count--)
			m_dynamic_actors.pop_back();
	}

	void ActorManager::RegisterActor(const SPtr<Actor>& actor)
	{
		m_ready_actors.push_back(actor);
	}
}
