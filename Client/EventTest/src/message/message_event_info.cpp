#include <include/client_core.h>
#include "message_event_info.h"

namespace event_test
{
	RotSpeedMessageEventInfo::RotSpeedMessageEventInfo(UINT event_id, float speed)
		: MessageEventInfo(event_id), m_speed(speed)
	{
	}
	SignInMessageEventInfo::SignInMessageEventInfo(UINT event_id, char* id, char* pw):
		MessageEventInfo(event_id)
	{
		strcpy_s(m_user_id, id);
		strcpy_s(m_user_pw, pw);

	}
}
