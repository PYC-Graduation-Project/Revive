#include"pch.h"
#include "player.h"
#include"iocp/iocp_server.h"
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
	if (m_socket == INVALID_SOCKET) return;
	EXP_OVER* ex_over = new EXP_OVER(COMP_OP::OP_SEND, num_bytes, mess);
	//std::cout <<"send_size :"<< (int)(((char*)mess)[0]) << std::endl;
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

void Player::ResetPlayer()
{
	if(m_room_id==-1)
		m_state = STATE::ST_FREE;
	m_is_active = false;
	is_matching = false;
	ZeroMemory(m_password, MAX_PASSWORD_SIZE + 1);
	m_hp = 100.0f;//추후 밸런스 조정
	m_maxhp = m_hp;
	m_mach_user_size = 0;
	m_socket = INVALID_SOCKET;
}
