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



void PacketManager::ProcessRecv(int client_id, EXP_OVER* exp_over, DWORD num_byte,SOCKET&socket)
{
	if (num_byte == 0)
	{
		cout << "받은게 없데" << endl;
	}
	LOG_INFO("패킷 사이즈{0}", num_byte);
	int remain_data = num_byte + m_prev_size;
	unsigned char* packet_start = exp_over->_net_buf;
	int packet_size = packet_start[0];
	LOG_INFO("ProcessPacket 처리시작");
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
	LOG_INFO("ProcessPacket 처리끝");
	DoRecv(socket);
}

void PacketManager::RegisterRecvFunction(char key, const std::function<void(int, unsigned char*)>& recv_func)
{
	m_recv_func_map[key] = recv_func;
}

void PacketManager::ProcessPacket(int c_id, unsigned char* p)
{
	unsigned char packet_type = p[1];
	if ((int)packet_type > 12)
	{
		
		LOG_INFO("처음보는 패킷타입:{0}", (int)packet_type);
	}
	auto iter = m_recv_func_map.find(packet_type);
	if (iter != m_recv_func_map.end())
	{
		//(this->*(iter->second))(c_id, p);
		(iter->second)(c_id, p);
	}
	//else
	//{
	//	//버그 찾았다
	//	LOG_INFO("없어용");
	//}
	
}

void PacketManager::DoRecv(SOCKET&socket)
{
	DWORD recv_flag = 0;
	
	ZeroMemory(&recv_over._wsa_over, sizeof(recv_over._wsa_over));
	recv_over._wsa_buf.buf = reinterpret_cast<char*>(recv_over._net_buf + m_prev_size);
	recv_over._wsa_buf.len = sizeof(recv_over._net_buf) - m_prev_size;
	int ret = WSARecv(socket, &recv_over._wsa_buf, 1, 0, &recv_flag, &recv_over._wsa_over, NULL);
	if (SOCKET_ERROR == ret) {
		int error_num = WSAGetLastError();
		if (ERROR_IO_PENDING != error_num)cout << "error" << endl;
			//Network::error_display(error_num);
	}
}




