#include"stdafx.h"
#include "packet_manager.h"
#include"network_move_object.h"
#include"network_obj_manager.h"
#include<iostream>
using namespace std;

PacketManager::PacketManager():m_id(0),m_prev_size(0)
{

}

PacketManager::~PacketManager()
{
}



void PacketManager::ProcessRecv(int client_id, EXP_OVER* exp_over, DWORD num_byte)
{
	int remain_data = num_byte + m_prev_size;
	unsigned char* packet_start = exp_over->_net_buf;
	int packet_size = packet_start[0];

	while (packet_size <= remain_data) {
		ProcessPacket(client_id, packet_start);
		remain_data -= packet_size;
		packet_start += packet_size;
		if (remain_data > 0) packet_size = packet_start[0];
		else break;
	}
	
	if (0 < remain_data) {
		m_prev_size = remain_data;
		memcpy(&exp_over->_net_buf, packet_start, remain_data);
	}
}

void PacketManager::RegisterRecvFunction(char key, const std::function<void(int, unsigned char*)>& recv_func)
{
	m_recv_func_map[key] = recv_func;
}

void PacketManager::ProcessPacket(int c_id, unsigned char* p)
{
	unsigned char packet_type = p[1];
	auto iter = m_recv_func_map.find(packet_type);
	if (iter != m_recv_func_map.end())
	{
		//(this->*(iter->second))(c_id, p);
		(iter->second)(c_id, p);
	};
	
}





