#include <include/client_core.h>
#include "message_event_info.h"

namespace event_test
{
	RotSpeedMessageEventInfo::RotSpeedMessageEventInfo(const std::string& name, float speed)
		: MessageEventInfo(name), m_speed(speed)
	{
	}
}
