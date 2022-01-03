#include <client/core/core.h>
#include <client/core/log.h>
#include "object/level/client_test_level.h"

namespace client_test
{
	ClientTestLevel::ClientTestLevel(std::string_view name)
		: Level(name)
	{
	}

	void ClientTestLevel::Initialize()
	{
		LOG_INFO("Initialize {0}", m_name);
	}

	void ClientTestLevel::Shutdown()
	{
		LOG_INFO("Shutdown {0}", m_name);
	}

	void ClientTestLevel::Update(float delta_time)
	{
		LOG_INFO("Delta time : {0}", delta_time);
	}
}
