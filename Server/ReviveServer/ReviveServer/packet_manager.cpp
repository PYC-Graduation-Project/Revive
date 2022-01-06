#include "pch.h"
#include "packet_manager.h"

#include"moveobj_manager.h"
using namespace std;

void PacketManager::Init()
{
	m_moveobj_manager = new MoveObjManager;
}

void PacketManager::ProcessPacket(int c_id, unsigned char* p)
{
	unsigned char packet_type = p[1];
	//Player* cl = reinterpret_cast<Player*>(clients[client_id]);

	switch (packet_type) {
	case CS_PACKET_LOGIN: {
		
		break;
	}
	case CS_PACKET_MOVE: {
		
		break;
	}
	case CS_PACKET_ATTACK: {
	
		break;
	}
	case CS_PACKET_TELEPORT: {


		break;
	}
	}
}

void PacketManager::ProcessAccept(HANDLE hiocp ,SOCKET& s_socket,EXP_OVER*exp_over)
{
	SOCKET c_socket = *(reinterpret_cast<SOCKET*>(exp_over->_net_buf));
	int new_id = m_moveobj_manager->GetNewID();
	if (-1 == new_id) {
		cout << "Maxmum user overflow. Accept aborted.\n";
		
	}
	else {//다시제작
		Player* cl = m_moveobj_manager->GetPlayer(new_id);
		cl->SetID( new_id);
		cl->Init(c_socket);
		CreateIoCompletionPort(reinterpret_cast<HANDLE>(c_socket), hiocp, new_id, 0);
		cl->DoRecv();
	}

	ZeroMemory(&exp_over->_wsa_over, sizeof(exp_over->_wsa_over));
	c_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
	*(reinterpret_cast<SOCKET*>(exp_over->_net_buf)) = c_socket;
	AcceptEx(s_socket, c_socket, exp_over->_net_buf + 8, 0, sizeof(SOCKADDR_IN) + 16,
		sizeof(SOCKADDR_IN) + 16, NULL, &exp_over->_wsa_over);
}

void PacketManager::ProcessLogin()
{
}

void PacketManager::ProcessAttack()
{
}

void PacketManager::ProcessMove()
{
}
