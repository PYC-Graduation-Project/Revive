#include "pch.h"
#include "ingame_server.h"
#include"packet_manager.h"
using namespace std;
InGameServer::InGameServer()
{
}

InGameServer::~InGameServer()
{
}

void InGameServer::Disconnect(int c_id)
{
}

bool InGameServer::OnAccept(EXP_OVER* exp_over)
{
	m_PacketManager->ProcessAccept(m_hiocp, m_s_socket, exp_over);
	return true;
}

bool InGameServer::OnRecv(int c_id, EXP_OVER* exp_over, DWORD num_bytes)
{
	if (num_bytes == 0) {
		Disconnect(c_id);
		return false;
	}
	//Player* cl = (Player*)clients[client_id];
	int remain_data = num_bytes; //+ cl->m_prev_size;
	unsigned char* packet_start = exp_over->_net_buf;
	int packet_size = packet_start[0];

	while (packet_size <= remain_data) {
		ProcessPacket(c_id, packet_start);
		remain_data -= packet_size;
		packet_start += packet_size;
		if (remain_data > 0) packet_size = packet_start[0];
		else break;
	}

	if (0 < remain_data) {
		//cl->m_prev_size = remain_data;
		memcpy(&exp_over->_net_buf, packet_start, remain_data);
	}
	//cl->do_recv();
}

void InGameServer::ProcessPacket(int c_id, unsigned char* p)
{
}

void InGameServer::Run()
{
	m_PacketManager = std::make_unique<PacketManager>();
	m_PacketManager->Init();
}
