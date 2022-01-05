#include "stdafx.h"
#include "client/object/level/core/level.h"
#include "client/object/actor/core/actor_manager.h"
#include "client/input/input.h"

namespace client_fw
{
	Level::Level(const std::string& name)
		: m_name(name), m_level_state(eLevelState::kInGame)
	{
		m_actor_manager = CreateUPtr<ActorManager>();
	}

	Level::~Level()
	{
	}

	void Level::InitializeLevel()
	{
		Initialize();
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
		m_actor_manager->RegisterActor(actor);
	}

	void Level::RegisterPressedEvent(std::string_view name, std::vector<EventKeyInfo>&& keys,
		const std::function<bool()>& func, bool consumption)
	{
		if (Input::RegisterPressedEvent(name, std::move(keys), func, consumption, eInputOwnerType::kLevel))
			RegisterInputEvent(name);
	}

	void Level::RegisterReleasedEvent(std::string_view name, std::vector<EventKeyInfo>&& keys,
		const std::function<bool()>& func, bool consumption)
	{
		if(Input::RegisterReleasedEvent(name, std::move(keys), func, consumption, eInputOwnerType::kLevel))
			RegisterInputEvent(name);
	}

	void Level::RegisterInputEvent(std::string_view name)
	{
		m_registered_input_event.emplace_back(std::move(name));
	}
}