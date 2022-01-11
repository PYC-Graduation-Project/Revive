#include"stdafx.h"
#include "packet_manager.h"
#include<iostream>
#include"Player.h"

void PacketManager::Init()
{

}

void PacketManager::ProcessPacket(CPlayer* player, unsigned char* p)
{
	unsigned char packet_type = p[1];
	//Player* cl = reinterpret_cast<Player*>(clients[client_id]);

	switch (packet_type) {
	case CS_PACKET_LOGIN: {
		
		break;
	}
	case CS_PACKET_MOVE: {
		
		break;
	}
	case CS_PACKET_ATTACK: {
	
		break;
	}
	case CS_PACKET_TELEPORT: {


		break;
	}
	}
}



void PacketManager::ProcessRecv(CPlayer* player , EXP_OVER*exp_over, DWORD num_bytes)
{

	CPlayer* cl = player;
	int remain_data = num_bytes+ cl->m_prev_size;
	unsigned char* packet_start = exp_over->_net_buf;
	int packet_size = packet_start[0];

	while (packet_size <= remain_data) {
		ProcessPacket(player, packet_start);
		remain_data -= packet_size;
		packet_start += packet_size;
		if (remain_data > 0) packet_size = packet_start[0];
		else break;
	}

	if (0 < remain_data) {
		cl->m_prev_size = remain_data;
		memcpy(&exp_over->_net_buf, packet_start, remain_data);
	}
	
}

void PacketManager::End()
{
	
}




void PacketManager::ProcessLogin()
{
}

void PacketManager::ProcessAttack()
{
}

void PacketManager::ProcessMove()
{
}
