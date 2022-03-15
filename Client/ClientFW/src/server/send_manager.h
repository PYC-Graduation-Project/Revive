#pragma once
#include"server/define.h"
#include <include/client_core.h>
#include<client/event/messageevent/message_event_info.h>
//class MessageEventInfo;
class SendManager
{


public:
	SendManager()=default ;
	virtual ~SendManager()=default;

	virtual void ProcessSend(const SOCKET& s_socket, const client_fw::SPtr<client_fw::MessageEventInfo>& message) {};

	void SendPacket(const SOCKET& s_socket, int num_byte, void* packet);

private:
	//SOCKET m_s_socket;
};

