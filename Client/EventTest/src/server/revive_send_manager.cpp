#include "stdafx.h"
#include "revive_send_manager.h"
#include"message/message_event_info.h"
void ReviveSendManager::ProcessSend(const SOCKET& s_socket, const client_fw::SPtr<client_fw::MessageEventInfo>& message)
{
	switch (message->GetEventID())
	{
	case HashCode("send sign in"):{
		auto msg = std::static_pointer_cast<event_test::SignInMessageEventInfo>(message);
		SendSignInPacket(s_socket, msg->GetUserID(), msg->GetUserPassword());
		break;
	}
	case HashCode("send sign up"): {
		auto msg = std::static_pointer_cast<event_test::SignUpMessageEventInfo>(message);
		SendSignUPPacket(s_socket, msg->GetUserID(), msg->GetUserPassword());
		break;
	}
	case HashCode("send sign matching"): {
		auto msg = std::static_pointer_cast<event_test::MatchingMessageEventInfo>(message);
		SendMatchingPacket(s_socket, msg->GetUserNum());
		break;
	}
	}
}

void ReviveSendManager::SendSignInPacket(const SOCKET& s_socket, char* id, char* pw)
{
	cs_packet_sign_in packet;
	packet.size = sizeof(packet);
	packet.type = CS_PACKET_SIGN_IN;
	strcpy_s(packet.name, id);
	strcpy_s(packet.password, pw);
	SendPacket(s_socket,sizeof(packet), &packet);
}

void ReviveSendManager::SendSignUPPacket(const SOCKET& s_socket, char* id, char* pw)
{
	cs_packet_sign_up packet;
	packet.size = sizeof(packet);
	packet.type = CS_PACKET_SIGN_UP;
	strcpy_s(packet.name, id);
	strcpy_s(packet.password, pw);
	SendPacket(s_socket, sizeof(packet), &packet);
}

void ReviveSendManager::SendMatchingPacket(const SOCKET& s_socket, int user_num)
{
	cs_packet_matching packet;
	packet.size = sizeof(packet);
	packet.type = CS_PACKET_MATCHING;
	packet.user_num = user_num;
	SendPacket(s_socket, sizeof(packet), &packet);
}

void ReviveSendManager::SendMovePacket(const SOCKET& s_socket, client_fw::Vec3& pos, client_fw::Quaternion& rot)
{
	cs_packet_move packet;
	packet.size = sizeof(packet);
	packet.type = CS_PACKET_MOVE;
	packet.x = pos.x;
	packet.y = pos.y;
	packet.z = pos.z;
	packet.r_x = rot.x;
	packet.r_y = rot.y;
	packet.r_z = rot.z;
	packet.r_w = rot.w;
	SendPacket(s_socket, sizeof(packet), &packet);
}