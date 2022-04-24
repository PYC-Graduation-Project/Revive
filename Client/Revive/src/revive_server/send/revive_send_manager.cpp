#include "stdafx.h"
#include "revive_send_manager.h"
#include"revive_server/message/message_event_info.h"
void ReviveSendManager::ProcessSend(const SOCKET& s_socket, const client_fw::SPtr<client_fw::MessageEventInfo>& message)
{
	switch (message->GetEventID())
	{
	case HashCode("send sign in"):{
		auto msg = std::static_pointer_cast<revive::SignInMessageEventInfo>(message);
		SendSignInPacket(s_socket, msg->GetUserID(), msg->GetUserPassword());
		break;
	}
	case HashCode("send sign up"): {
		auto msg = std::static_pointer_cast<revive::SignUpMessageEventInfo>(message);
		SendSignUPPacket(s_socket, msg->GetUserID(), msg->GetUserPassword());
		break;
	}
	case HashCode("send sign matching"): {
		auto msg = std::static_pointer_cast<revive::MatchingMessageEventInfo>(message);
		SendMatchingPacket(s_socket, msg->GetUserNum());
		break;
	}
	case HashCode("send attack"): {
		auto msg = std::static_pointer_cast<revive::SendAttackEventInfo>(message);
		SendAttackPacket(s_socket);
		break;
	}
	case HashCode("send hit"): {
		auto msg = std::static_pointer_cast<revive::ObjectHitMessageEventInfo>(message);
		SendHitPacket(s_socket,msg->GetVictimID(),msg->GetAttackerID());
		break;
	}
	}
}

void ReviveSendManager::SendMovePacket(const SOCKET& s_socket, const client_fw::Vec3& position, const client_fw::Quaternion& rotation)
{
	cs_packet_move packet;
	packet.size = sizeof(packet);
	packet.type = CS_PACKET_MOVE;
	packet.x = position.x;
	packet.y = position.y;
	packet.z = position.z;
	packet.r_x = rotation.x;
	packet.r_y = rotation.y;
	packet.r_z = rotation.z;
	packet.r_w = rotation.w;
	SendPacket(s_socket, sizeof(packet), &packet);
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

void ReviveSendManager::SendAttackPacket(const SOCKET& s_socket)
{
	cs_packet_attack packet;
	packet.size = sizeof(packet);
	packet.type = CS_PACKET_ATTACK;
	SendPacket(s_socket, sizeof(packet), &packet);
}

void ReviveSendManager::SendHitPacket(const SOCKET& s_socket,int obj_id,int attacker_id)
{
	cs_packet_hit packet;
	packet.size = sizeof(packet);
	packet.type = CS_PACKET_HIT;
	packet.victim_id = obj_id;
	packet.attacker_id = attacker_id;
	SendPacket(s_socket, sizeof(packet), &packet);
}
