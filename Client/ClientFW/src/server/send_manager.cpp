#include "stdafx.h"
#include "send_manager.h"
#include"server/network.h"


void SendManager::SendPacket(const SOCKET& s_socket,int num_byte, void* packet)
{
	EXP_OVER* exp_over = new EXP_OVER(COMP_OP::OP_SEND, num_byte, packet);
	int ret = WSASend(s_socket, &exp_over->_wsa_buf, 1, 0, 0, &exp_over->_wsa_over, NULL);
	if (SOCKET_ERROR == ret) {
		int error_num = WSAGetLastError();
		if (ERROR_IO_PENDING != error_num)
			LOG_INFO("send 오류발생{0}",error_num);
	}
}

