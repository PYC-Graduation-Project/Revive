#include"pch.h"
#include "player.h"
#include"iocp_server.h"
void Player::DoRecv()
{
	DWORD recv_flag = 0;
	ZeroMemory(&m_recv_over._wsa_over, sizeof(m_recv_over._wsa_over));
	m_recv_over._wsa_buf.buf = reinterpret_cast<char*>(m_recv_over._net_buf + m_prev_size);
	m_recv_over._wsa_buf.len = sizeof(m_recv_over._net_buf) - m_prev_size;
	int ret = WSARecv(m_socket, &m_recv_over._wsa_buf, 1, 0, &recv_flag, &m_recv_over._wsa_over, NULL);
	if (SOCKET_ERROR == ret) {
		int error_num = WSAGetLastError();
		if (ERROR_IO_PENDING != error_num)
			IOCPServer::error_display(error_num);
	}
}

void Player::DoSend(int num_bytes, void* mess)
{
	EXP_OVER* ex_over = new EXP_OVER(COMP_OP::OP_SEND, num_bytes, mess);
	int ret = WSASend(m_socket, &ex_over->_wsa_buf, 1, 0, 0, &ex_over->_wsa_over, NULL);
	if (SOCKET_ERROR == ret) {
		int error_num = WSAGetLastError();
		if (ERROR_IO_PENDING != error_num)
			IOCPServer::error_display(error_num);
	}
}

void Player::Init(SOCKET&c_socket)
{
	m_prev_size = 0;
	m_recv_over._comp_op = COMP_OP::OP_RECV;
	m_recv_over._wsa_buf.buf = reinterpret_cast<char*>(m_recv_over._net_buf);
	m_recv_over._wsa_buf.len = sizeof(m_recv_over._net_buf);
	ZeroMemory(&m_recv_over._wsa_over, sizeof(m_recv_over._wsa_over));
	m_socket = c_socket;

}
