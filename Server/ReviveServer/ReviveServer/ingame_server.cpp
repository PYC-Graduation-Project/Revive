#include "pch.h"
#include "ingame_server.h"
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

bool InGameServer::OnAccept(int new_id, EXP_OVER* exp_over)
{
	SOCKET c_socket = *(reinterpret_cast<SOCKET*>(exp_over->_net_buf));
	
	if (-1 == new_id) {
		cout << "Maxmum user overflow. Accept aborted.\n";
		return false;
	}
	else {//다시제작
		//Player* cl = (Player*)clients[new_id];
		//cl->id = new_id;
		//cout << new_id << endl;
		//cl->m_prev_size = 0;
		//cl->m_recv_over._comp_op = COMP_OP::OP_RECV;
		//cl->m_recv_over._wsa_buf.buf = reinterpret_cast<char*>(cl->m_recv_over._net_buf);
		//cl->m_recv_over._wsa_buf.len = sizeof(cl->m_recv_over._net_buf);
		//ZeroMemory(&cl->m_recv_over._wsa_over, sizeof(cl->m_recv_over._wsa_over));
		//cl->m_socket = c_socket;
		//
		//CreateIoCompletionPort(reinterpret_cast<HANDLE>(c_socket), m_hiocp, new_id, 0);
		//cl->do_recv();
	}

	ZeroMemory(&exp_over->_wsa_over, sizeof(exp_over->_wsa_over));
	c_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
	*(reinterpret_cast<SOCKET*>(exp_over->_net_buf)) = c_socket;
	AcceptEx(m_s_socket, c_socket, exp_over->_net_buf + 8, 0, sizeof(SOCKADDR_IN) + 16,
		sizeof(SOCKADDR_IN) + 16, NULL, &exp_over->_wsa_over);
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
