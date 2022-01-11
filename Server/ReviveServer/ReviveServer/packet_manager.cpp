#include "pch.h"
#include "packet_manager.h"

#include"moveobj_manager.h"
using namespace std;

void PacketManager::Init()
{
	m_moveobj_manager = new MoveObjManager;
	m_moveobj_manager->InitPlayer();
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
	cout << "억셉트처리옴" << endl;
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

void PacketManager::ProcessRecv(int c_id , EXP_OVER*exp_over, DWORD num_bytes)
{

	if (num_bytes == 0) {
		m_moveobj_manager->Disconnect(c_id);
		
	}
	Player* cl = m_moveobj_manager->GetPlayer(c_id);
	int remain_data = num_bytes+ cl->m_prev_size;
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
		cl->m_prev_size = remain_data;
		memcpy(&exp_over->_net_buf, packet_start, remain_data);
	}
	cl->DoRecv();
}

void PacketManager::UpdateObjMove()
{
	for (int i = 0; i < MAX_USER; ++i)
	{
		for (int j = 0; j <= NPC_ID_END; ++j) {
			//이후에 조건 추가
			if(i!=j)
			SendMovePacket(i, j);
		}
	}
	SetTimerEvent(0, 0, EVENT_TYPE::EVENT_PLAYER_MOVE, 10);
}

void PacketManager::SendMovePacket(int c_id, int mover)
{
	sc_packet_move packet;
	MoveObj* p = m_moveobj_manager->GetMoveObj(mover);
	packet.id = mover;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_MOVE;

	packet.x =p->GetPosX();
	packet.y =p->GetPosY();
	packet.z =p->GetPosZ();

	packet.right_x = p->GetRightVec().x;
	packet.right_y = p->GetRightVec().y;
	packet.right_z = p->GetRightVec().z;

	packet.look_x = p->GetLookVec().x;
	packet.look_y = p->GetLookVec().y;
	packet.look_z = p->GetLookVec().z;
	
	m_moveobj_manager->GetPlayer(c_id)->DoSend(sizeof(packet), &packet);
}

timer_event PacketManager::SetTimerEvent(int obj_id, int target_id, EVENT_TYPE ev, int seconds)
{
	timer_event t;
	t.obj_id = obj_id;
	t.target_id = target_id;
	t.ev = ev;
	t.start_time = chrono::system_clock::now() + (1ms * seconds);
	return t;
}

void PacketManager::End()
{
	m_moveobj_manager->DestroyObject();
}

void PacketManager::Disconnect(int c_id)
{
	m_moveobj_manager->Disconnect(c_id);
}



void PacketManager::ProcessLogin(int c_id,unsigned char* p)
{
}

void PacketManager::ProcessAttack(int c_id,unsigned char* p)
{
}

void PacketManager::ProcessMove(int c_id,unsigned char* p)
{
	cs_packet_move* packet = reinterpret_cast<cs_packet_move*>(p);
	Player* cl = m_moveobj_manager->GetPlayer(c_id);
	Vector3 pos{ cl->GetPosX(),cl->GetPosY(),cl->GetPosZ() };
	Vector3 look{ cl->GetLookVec()};
	Vector3 right{ cl->GetRightVec() };
	switch (packet->direction)
	{
	case 0:
	{
		//pos
		break;
	}
	case 1:
	{
		break;
	}
	case 2:
	{
		break;
	}
	case 3:
	{
		break;
	}
	}
}
