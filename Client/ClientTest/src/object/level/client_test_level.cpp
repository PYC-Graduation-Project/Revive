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
			[this]()->bool {m_pos += Vec2(0.0f, 1.0f); return true; });
		RegisterPressedEvent("move_right_side", { EventKeyInfo{eKey::kD} },
			[this]()->bool {m_pos += Vec2(1.0f, 0.0f); return true;  });
		RegisterPressedEvent("move_left_side", { EventKeyInfo{eKey::kA} },
			[this]()->bool {m_pos += Vec2(-1.0f, 0.0f); return true;  });
		RegisterPressedEvent("move_backward", { EventKeyInfo{eKey::kS} },
			[this]()->bool {m_pos += Vec2(0.0f, -1.0f);  return true; });
		RegisterReleasedEvent("check_pos", { EventKeyInfo{eKey::kF} },
			[this]()->bool {LOG_TRACE(m_pos); return true;  });
	}

	void ClientTestLevel::Shutdown()
	{
		LOG_INFO("Shutdown {0}", m_name);
	}

	void ClientTestLevel::Update(float delta_time)
	{
	}
}
