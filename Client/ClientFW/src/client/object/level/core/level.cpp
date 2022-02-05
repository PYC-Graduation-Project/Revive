#include "stdafx.h"
#include "client/object/level/core/level.h"
#include "client/object/actor/core/actor_manager.h"
#include "client/object/actor/core/actor.h"
#include "client/input/input.h"
#include "client/util/octree/mesh_octree.h"

namespace client_fw
{
	Level::Level(const std::string& name)
		: m_name(name), m_level_state(eLevelState::kInGame)
		, m_is_runtime_level(false)
	{
		m_actor_manager = CreateUPtr<ActorManager>();
	}

	Level::~Level()
	{
	}

	bool Level::InitializeLevel()
	{
		return Initialize();
	}

	void Level::ShutdownLevel()
	{
		for (auto name : m_registered_input_event)
			Input::UnregisterInputEvent(name);

		m_actor_manager->Shutdown();

		Shutdown();
	}

	void Level::UpdateLevel(float delta_time)
	{
		Update(delta_time);

		m_actor_manager->Update(delta_time);
	}

	void Level::SpawnActor(const SPtr<Actor>& actor)
	{
		if (IsRuntime())
		{
			switch (actor->GetMobilityState())
			{
			case eMobilityState::kStatic:
				LOG_WARN("Static actor[{0}] cannot be spawned at runtime", actor->GetName());
				break;
			case eMobilityState::kDestructable:
			case eMobilityState::kMovable:
				m_actor_manager->RegisterActor(actor);
				break;
			}
		}
		else
		{
			m_actor_manager->RegisterActor(actor);
		}
	}

	void Level::RegisterPressedEvent(const std::string& name, std::vector<EventKeyInfo>&& keys,
		const std::function<bool()>& func, bool consumption)
	{
		if (Input::RegisterPressedEvent(name, std::move(keys), func, consumption, eInputOwnerType::kLevel))
			RegisterInputEvent(name);
	}

	void Level::RegisterReleasedEvent(const std::string& name, std::vector<EventKeyInfo>&& keys,
		const std::function<bool()>& func, bool consumption)
	{
		if(Input::RegisterReleasedEvent(name, std::move(keys), func, consumption, eInputOwnerType::kLevel))
			RegisterInputEvent(name);
	}

	void Level::RegisterInputEvent(const std::string& name)
	{
		m_registered_input_event.push_back(name);
	}

	std::vector<SPtr<VisualOctree>> Level::CreateVisualOctrees() const
	{
		std::vector<SPtr<VisualOctree>> visual_octrees;
		visual_octrees.emplace_back(CreateSPtr<VisualOctree>(10000.0f, vec3::ZERO, 0));
		return visual_octrees;
	}

	std::vector<SPtr<CollisionOctree>> Level::CreateCollisionOctrees() const
	{
		std::vector<SPtr<CollisionOctree>> octrees;
		octrees.emplace_back(CreateSPtr<CollisionOctree>(10000.0f, vec3::ZERO, 0));
		return octrees;
	}
}