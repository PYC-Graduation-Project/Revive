#include "stdafx.h"
#include "network.h"

bool Network::Init()
{
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
		return false;
	m_s_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == m_s_socket)
	{
		error_display(WSAGetLastError());
		return true;
	}
	m_hiocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, NULL, 0);
	

	
    return true;
}

bool Network::Connect()
{
	SOCKADDR_IN server_addr;
	ZeroMemory(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = inet_addr("192.168.219.187");
	int retval = WSAConnect(m_s_socket, reinterpret_cast<sockaddr*>(&server_addr),
		sizeof(server_addr), NULL, NULL, NULL, NULL);
	if (0 != retval) {
		error_display(WSAGetLastError());
		return false;
	}
	m_prev_size = 0;
	ZeroMemory(&recv_over, sizeof(recv_over));
	recv_over._comp_op = COMP_OP::OP_RECV;
	recv_over._wsa_buf.buf = reinterpret_cast<CHAR*>(recv_over._net_buf);
	recv_over._wsa_buf.len = sizeof(recv_over._net_buf);
	DWORD flag = 0;

	CreateIoCompletionPort(reinterpret_cast<HANDLE>(m_s_socket), m_hiocp, m_id, 0);

	int ret = WSARecv(m_s_socket, &recv_over._wsa_buf, 1,
		NULL, &flag, &recv_over._wsa_over, NULL);
	if (SOCKET_ERROR == ret) {
		int err_no = WSAGetLastError();
		if (err_no != WSA_IO_PENDING)
		{
			error_display(err_no);
			return false;
		}
	}
		
	return false;
}

void Network::Worker()
{
	
		for (;;) {
			DWORD num_byte;
			LONG64 iocp_key;
			WSAOVERLAPPED* p_over;
			BOOL ret = GetQueuedCompletionStatus(m_hiocp, &num_byte, (PULONG_PTR)&iocp_key, &p_over, INFINITE);
			cout << "GQCS returned.\n";
			int client_id = static_cast<int>(iocp_key);
			EXP_OVER* exp_over = reinterpret_cast<EXP_OVER*>(p_over);
			if (FALSE == ret) {
				
				if (exp_over->_comp_op == COMP_OP::OP_SEND)
					delete exp_over;
				continue;
			}
			switch (exp_over->_comp_op) {
			case COMP_OP::OP_RECV: {
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
				DoRecv();
				break;
			}
			case COMP_OP::OP_SEND: {
				if (num_byte != exp_over->_wsa_buf.len) {
					// DISCONNECT();
				}
				delete exp_over;
				break;
			}
			
			}
		}
	
}

void Network::DoRecv()
{
	DWORD recv_flag = 0;
	ZeroMemory(&recv_over._wsa_over, sizeof(recv_over._wsa_over));
	recv_over._wsa_buf.buf = reinterpret_cast<char*>(recv_over._net_buf + m_prev_size);
	recv_over._wsa_buf.len = sizeof(recv_over._net_buf) - m_prev_size;
	int ret = WSARecv(m_s_socket, &recv_over._wsa_buf, 1, 0, &recv_flag, &recv_over._wsa_over, NULL);
	if (SOCKET_ERROR == ret) {
		int error_num = WSAGetLastError();
		if (ERROR_IO_PENDING != error_num)
			error_display(error_num);
	}
}

void Network::ProcessPacket(int c_id, unsigned char* p)
{
	unsigned char packet_type = p[1];
	switch (packet_type)
	{
	case SC_PACKET_MOVE: {
		sc_packet_move* move_packet = reinterpret_cast<sc_packet_move*>(p);
		pos = XMFLOAT3(move_packet->x, move_packet->y, move_packet->z);
		look= XMFLOAT3(move_packet->look_x, move_packet->look_y, move_packet->look_z);
		right= XMFLOAT3(move_packet->right_x, move_packet->right_y, move_packet->right_z);
	}
	}
}
