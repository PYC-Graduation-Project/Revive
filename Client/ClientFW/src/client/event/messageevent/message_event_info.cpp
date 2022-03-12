#include "stdafx.h"
#include "client/event/messageevent/message_event_info.h"

namespace client_fw
{
	bool operator<(const MessageEventInfo& msg1, const MessageEventInfo& msg2)
	{
		return msg1.GetName() < msg2.GetName();
	}
}


