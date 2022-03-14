#pragma once
#include"server/define.h"
#include <include/client_core.h>
#include<client/event/messageevent/message_event_info.h>
//class MessageEventInfo;
class SendManager
{


public:
	SendManager() :m_s_socket(INVALID_SOCKET) {};
	virtual ~SendManager()=default;

	virtual void SendByMessageToSrver(const client_fw::SPtr<client_fw::MessageEventInfo>& message) {};

	void SendPacket(int num_byte, void* packet);

private:
	SOCKET m_s_socket;
};

