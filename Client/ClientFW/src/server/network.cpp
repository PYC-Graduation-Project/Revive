#include"stdafx.h"
#include "network.h"
#include"server/packet_manager.h"
using namespace std;
Network* Network::m_pInst = NULL;

bool Network::Init()
{
	m_id = 0;
	m_packet_manager = std::make_unique<PacketManager>();
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
		return false;
	m_packet_manager->GetSock() = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == m_packet_manager->GetSock())
	{
		m_packet_manager->error_display(WSAGetLastError());
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
	int retval = WSAConnect(m_packet_manager->GetSock(), reinterpret_cast<sockaddr*>(&server_addr),
		sizeof(server_addr), NULL, NULL, NULL, NULL);
	if (0 != retval) {
		m_packet_manager->error_display(WSAGetLastError());
		return false;
	}
	m_prev_size = 0;
	ZeroMemory(&recv_over, sizeof(recv_over));
	recv_over._comp_op = COMP_OP::OP_RECV;
	recv_over._wsa_buf.buf = reinterpret_cast<CHAR*>(recv_over._net_buf);
	recv_over._wsa_buf.len = sizeof(recv_over._net_buf);
	DWORD flag = 0;
	m_hiocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, NULL, 0);
	CreateIoCompletionPort(reinterpret_cast<HANDLE>(m_packet_manager->GetSock()), m_hiocp, m_id, 0);

	int ret = WSARecv(m_packet_manager->GetSock(), &recv_over._wsa_buf, 1,
		NULL, &flag, &recv_over._wsa_over, NULL);
	if (SOCKET_ERROR == ret) {
		int err_no = WSAGetLastError();
		if (err_no != WSA_IO_PENDING)
		{
			m_packet_manager->error_display(err_no);
			return false;
		}
	}
		
	return false;
}



void Network::SendSignInPacket(char*id, char*pw)
{
	cs_packet_sign_in packet;
	packet.size = sizeof(packet);
	packet.type = CS_PACKET_SIGN_IN;
	strcpy_s(packet.name, id);
	strcpy_s(packet.password, pw);
	m_packet_manager->SendPacket(sizeof(packet), &packet);
}

void Network::SendSignUPPacket(char*id, char*pw)
{
	cs_packet_sign_up packet;
	packet.size = sizeof(packet);
	packet.type = CS_PACKET_SIGN_UP;
	strcpy_s(packet.name, id);
	strcpy_s(packet.password, pw);
	m_packet_manager->SendPacket(sizeof(packet), &packet);
}

void Network::SendMatchingPacket(int user_num)
{
	cs_packet_matching packet;
	packet.size = sizeof(packet);
	packet.type = CS_PACKET_MATCHING;
	packet.user_num = user_num;
	m_packet_manager->SendPacket(sizeof(packet), &packet);
}

void Network::SendMovePacket(client_fw::Vec3& pos, client_fw::Quaternion& rot)
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
	m_packet_manager->SendPacket(sizeof(packet), &packet);
}

void Network::Worker()
{
	Connect();
		for (;;) {
			DWORD num_byte;
			LONG64 iocp_key;
			WSAOVERLAPPED* p_over;
			BOOL ret = GetQueuedCompletionStatus(m_hiocp, &num_byte, (PULONG_PTR)&iocp_key, &p_over, INFINITE);
			//cout << "GQCS returned.\n";
			m_packet_manager->error_display(WSAGetLastError());
			int client_id = static_cast<int>(iocp_key);
			EXP_OVER* exp_over = reinterpret_cast<EXP_OVER*>(p_over);
			if (FALSE == ret) {
				int err_no = WSAGetLastError();
				if (64 == err_no)closesocket(m_packet_manager->GetSock());
				else {
					m_packet_manager->error_display(err_no);
					closesocket(m_packet_manager->GetSock());
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
					closesocket(m_packet_manager->GetSock());
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
	int ret = WSARecv(m_packet_manager->GetSock(), &recv_over._wsa_buf, 1, 0, &recv_flag, &recv_over._wsa_over, NULL);
	if (SOCKET_ERROR == ret) {
		int error_num = WSAGetLastError();
		if (ERROR_IO_PENDING != error_num)
			m_packet_manager->error_display(error_num);
	}
}

void Network::OnRecv(int client_id,EXP_OVER*exp_over,DWORD num_byte)
{
	m_packet_manager->ProcessRecv(client_id, exp_over, num_byte);
}

