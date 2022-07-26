#include"stdafx.h"
#include "network.h"

#include<client/core/application.h>

#include"packet_manager.h"

using namespace std;
Network* Network::m_pInst = NULL;
bool Network::matching_end = false;
bool Network::Init(client_fw::UPtr<PacketManager>&& packet_manager, client_fw::UPtr<SendManager>&& send_manager)
{
	m_id = 0;
	m_move_time = chrono::system_clock::now();
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
	inet_pton(AF_INET,"112.170.181.162", &server_addr.sin_addr);
	int retval = WSAConnect(m_s_socket, reinterpret_cast<sockaddr*>(&server_addr),
		sizeof(server_addr), NULL, NULL, NULL, NULL);
	if (0 != retval) {
		Network::error_display(WSAGetLastError());
		return false;
	}
	
	m_hiocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, NULL, 0);
	CreateIoCompletionPort(reinterpret_cast<HANDLE>(m_s_socket), m_hiocp, m_id, 0);
	m_packet_manager->DoRecv(m_s_socket);

		
	return true;
}





void Network::Worker()
{
	if (false == Connect())
	{
		LOG_INFO("Faild to Connect");
		return;
	}
		while (m_iswork) {
			DWORD num_byte;
			LONG64 iocp_key;
			WSAOVERLAPPED* p_over;
			BOOL ret = GetQueuedCompletionStatus(m_hiocp, &num_byte, (PULONG_PTR)&iocp_key, &p_over, INFINITE);
			//cout << "GQCS returned.\n";
			//Network::error_display(WSAGetLastError());
			//LOG_INFO(WSAGetLastError());
			int client_id = static_cast<int>(iocp_key);
			EXP_OVER* exp_over = reinterpret_cast<EXP_OVER*>(p_over);
			if (FALSE == ret) {
				int err_no = WSAGetLastError();
					Network::error_display(err_no);
					closesocket(m_s_socket);
					LOG_INFO("Network off");
					if (exp_over->_comp_op == COMP_OP::OP_SEND)
					{
						delete exp_over;
					}
					break;
			}
			switch (exp_over->_comp_op) {
			case COMP_OP::OP_RECV: {
				//LOG_INFO("계속 recv중");
				OnRecv(client_id, exp_over, num_byte,m_s_socket);
				
				break;
			}
			case COMP_OP::OP_SEND: {
				if (num_byte != exp_over->_wsa_buf.len) {
					closesocket(m_s_socket);
					LOG_INFO("삑남");
					delete exp_over;
				}
				else
				{
					//LOG_INFO("send 완료");
					delete exp_over;
				}
				break;
			}
			
			}
		}
		LOG_INFO("워커 쓰레드 종료");
}



void Network::OnRecv(int client_id,EXP_OVER*exp_over,DWORD num_byte, SOCKET& socket)
{
	m_packet_manager->ProcessRecv(client_id, exp_over, num_byte,socket);
}

void Network::SendMessageToServer(const client_fw::SPtr<client_fw::MessageEventInfo>& message)
{
	m_send_manager->ProcessSend(m_s_socket,message);
}

void Network::SendMovePacket(const client_fw::Vec3& position, const client_fw::Quaternion& rotation)
{
	auto end_t = std::chrono::system_clock::now();
	if (m_move_time <= end_t) {
		m_send_manager->SendMovePacket(m_s_socket, position, rotation);
		m_move_time = end_t + 50ms;
	}
}

