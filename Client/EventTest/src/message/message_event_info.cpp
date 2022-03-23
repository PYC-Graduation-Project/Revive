#include <include/client_core.h>
#include "message_event_info.h"

namespace event_test
{
	RotSpeedMessageEventInfo::RotSpeedMessageEventInfo(UINT event_id, float speed)
		: MessageEventInfo(event_id), m_speed(speed)
	{
	}
}
