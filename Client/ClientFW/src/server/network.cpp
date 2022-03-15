#include"stdafx.h"
#include "network.h"
#include<client/core/application.h>
using namespace std;
Network* Network::m_pInst = NULL;

bool Network::Init(client_fw::UPtr<PacketManager>&& packet_manager, client_fw::UPtr<SendManager>&& send_manager)
{
	m_id = 0;
	m_packet_manager = move(packet_manager);
	m_send_manager = move(send_manager);
	m_packet_manager->Init();
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
		return false;
	m_s_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == m_s_socket)
	{
		Network::error_display(WSAGetLastError());
		return false;
	}
	
	
	
	
    return true;
}

bool Network::Connect()
{
	SOCKADDR_IN server_addr;
	ZeroMemory(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET,"127.0.0.1", &server_addr.sin_addr);
	int retval = WSAConnect(m_s_socket, reinterpret_cast<sockaddr*>(&server_addr),
		sizeof(server_addr), NULL, NULL, NULL, NULL);
	if (0 != retval) {
		Network::error_display(WSAGetLastError());
		return false;
	}
	m_prev_size = 0;
	ZeroMemory(&recv_over, sizeof(recv_over));
	recv_over._comp_op = COMP_OP::OP_RECV;
	recv_over._wsa_buf.buf = reinterpret_cast<CHAR*>(recv_over._net_buf);
	recv_over._wsa_buf.len = sizeof(recv_over._net_buf);
	DWORD flag = 0;
	m_hiocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, NULL, 0);
	CreateIoCompletionPort(reinterpret_cast<HANDLE>(m_s_socket), m_hiocp, m_id, 0);

	int ret = WSARecv(m_s_socket, &recv_over._wsa_buf, 1,
		NULL, &flag, &recv_over._wsa_over, NULL);
	if (SOCKET_ERROR == ret) {
		int err_no = WSAGetLastError();
		if (err_no != WSA_IO_PENDING)
		{
			Network::error_display(err_no);
			return false;
		}
	}
		
	return false;
}





void Network::Worker()
{
	Connect();
		while (m_iswork) {
			DWORD num_byte;
			LONG64 iocp_key;
			WSAOVERLAPPED* p_over;
			BOOL ret = GetQueuedCompletionStatus(m_hiocp, &num_byte, (PULONG_PTR)&iocp_key, &p_over, INFINITE);
			//cout << "GQCS returned.\n";
			Network::error_display(WSAGetLastError());
			int client_id = static_cast<int>(iocp_key);
			EXP_OVER* exp_over = reinterpret_cast<EXP_OVER*>(p_over);
			if (FALSE == ret) {
				int err_no = WSAGetLastError();
				if (64 == err_no)closesocket(m_s_socket);
				else {
					Network::error_display(err_no);
					closesocket(m_s_socket);
				}
				
				continue;
			}
			switch (exp_over->_comp_op) {
			case COMP_OP::OP_RECV: {
				OnRecv(client_id, exp_over, num_byte);
				DoRecv();
				break;
			}
			case COMP_OP::OP_SEND: {
				if (num_byte != exp_over->_wsa_buf.len) {
					closesocket(m_s_socket);
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
	int pr_size = m_packet_manager->GetPrevSize();
	ZeroMemory(&recv_over._wsa_over, sizeof(recv_over._wsa_over));
	recv_over._wsa_buf.buf = reinterpret_cast<char*>(recv_over._net_buf + pr_size);
	recv_over._wsa_buf.len = sizeof(recv_over._net_buf) - pr_size;
	int ret = WSARecv(m_s_socket, &recv_over._wsa_buf, 1, 0, &recv_flag, &recv_over._wsa_over, NULL);
	if (SOCKET_ERROR == ret) {
		int error_num = WSAGetLastError();
		if (ERROR_IO_PENDING != error_num)
			Network::error_display(error_num);
	}
}

void Network::OnRecv(int client_id,EXP_OVER*exp_over,DWORD num_byte)
{
	m_packet_manager->ProcessRecv(client_id, exp_over, num_byte);
}

void Network::SendMessageToServer(const client_fw::SPtr<client_fw::MessageEventInfo>& message)
{
	m_send_manager->ProcessSend(m_s_socket,message);
}

