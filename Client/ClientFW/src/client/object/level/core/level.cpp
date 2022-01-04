#include "stdafx.h"
#include "client/object/level/core/level.h"
#include "client/input/input.h"

namespace client_fw
{
	Level::Level(std::string_view name)
		: m_name(name), m_level_state(eLevelState::kInGame)
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

		Shutdown();
	}

	void Level::UpdateLevel(float delta_time)
	{
		Update(delta_time);
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