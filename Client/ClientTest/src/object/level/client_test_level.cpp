#include <client/core/core.h>
#include <client/core/log.h>
#include "object/level/client_test_level.h"
#include "client/input/input.h"

namespace client_test
{
	ClientTestLevel::ClientTestLevel(std::string_view name)
		: Level(name)
	{
	}

	void ClientTestLevel::Initialize()
	{
		LOG_INFO("Initialize {0}", m_name);

		RegisterPressedEvent("move_forward", { EventKeyInfo{eKey::kW} },
			[this]() {m_pos += Vec2(0.0f, 1.0f); });
		RegisterPressedEvent("move_right_side", { EventKeyInfo{eKey::kD} },
			[this]() {m_pos += Vec2(1.0f, 0.0f); });
		RegisterPressedEvent("move_left_side", { EventKeyInfo{eKey::kA} },
			[this]() {m_pos += Vec2(-1.0f, 0.0f); });
		RegisterPressedEvent("move_backward", { EventKeyInfo{eKey::kS} },
			[this]() {m_pos += Vec2(0.0f, -1.0f); });
		RegisterReleasedEvent("check_pos", { EventKeyInfo{eKey::kF} },
			[this]() {LOG_TRACE(m_pos); });
	}

	void ClientTestLevel::Shutdown()
	{
		LOG_INFO("Shutdown {0}", m_name);
	}

	void ClientTestLevel::Update(float delta_time)
	{
	}
}
