#include "stdafx.h"
#include "revive_send_manager.h"

void ReviveSendManager::SendByMessageToSrver(const client_fw::SPtr<client_fw::MessageEventInfo>& message)
{
	//switch (message->GetEventID())
	//{
	//
	//}
}

void ReviveSendManager::SendSignInPacket(char* id, char* pw)
{
	cs_packet_sign_in packet;
	packet.size = sizeof(packet);
	packet.type = CS_PACKET_SIGN_IN;
	strcpy_s(packet.name, id);
	strcpy_s(packet.password, pw);
	SendPacket(sizeof(packet), &packet);
}

void ReviveSendManager::SendSignUPPacket(char* id, char* pw)
{
	cs_packet_sign_up packet;
	packet.size = sizeof(packet);
	packet.type = CS_PACKET_SIGN_UP;
	strcpy_s(packet.name, id);
	strcpy_s(packet.password, pw);
	SendPacket(sizeof(packet), &packet);
}

void ReviveSendManager::SendMatchingPacket(int user_num)
{
	cs_packet_matching packet;
	packet.size = sizeof(packet);
	packet.type = CS_PACKET_MATCHING;
	packet.user_num = user_num;
	SendPacket(sizeof(packet), &packet);
}

void ReviveSendManager::SendMovePacket(client_fw::Vec3& pos, client_fw::Quaternion& rot)
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
	SendPacket(sizeof(packet), &packet);
}