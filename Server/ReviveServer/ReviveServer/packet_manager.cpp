#include "pch.h"
#include "packet_manager.h"
#include"db.h"
#include"moveobj_manager.h"
#include"room_manager.h"
#include"room.h"
using namespace std;

void PacketManager::Init()
{
	m_moveobj_manager = new MoveObjManager;
	m_moveobj_manager->InitPlayer();
	m_room_manager = new RoomManager;
	m_room_manager->InitRoom();
	m_db = new DB;
	m_db->Init();
	m_db2 = new DB;
	m_db2->Init();
}

void PacketManager::ProcessPacket(int c_id, unsigned char* p)
{
	unsigned char packet_type = p[1];
	//Player* cl = reinterpret_cast<Player*>(clients[client_id]);

	switch (packet_type) {
	case CS_PACKET_SIGN_IN: {
		ProcessSignIn(c_id, p);
		break;
	}
	case CS_PACKET_SIGN_UP: {
		ProcessSignUp(c_id, p);
		break;
	}
	case CS_PACKET_MOVE: {
		ProcessMove(c_id, p);		
		break;
	}
	case CS_PACKET_ATTACK: {
		ProcessAttack(c_id, p);
		break;
	}
	case CS_PACKET_MATCHING: {
		ProcessMatching(c_id, p);
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
		cl->SetID(new_id);
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

void PacketManager::SendLoginFailPacket(int c_id, int reason)
{
	sc_packet_login_fail packet;
	Player* pl = m_moveobj_manager->GetPlayer(c_id);
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_LOGIN_FAIL;
	packet.reason = reason;
	pl->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendSignInOK(int c_id)
{
	
}

void PacketManager::SendSignUpOK(int c_id)
{
	sc_packet_sign_up_ok packet;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_SIGN_UP_OK;
	m_moveobj_manager->GetPlayer(c_id)->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendMatchingOK(int c_id)
{
	sc_packet_matching packet;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_MATCHING;
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
	if (m_moveobj_manager)delete m_moveobj_manager;
	if (m_db)delete m_db;
	if (m_db2)delete m_db2;
}

void PacketManager::Disconnect(int c_id)
{
	m_moveobj_manager->Disconnect(c_id);
}



void PacketManager::ProcessSignIn(int c_id,unsigned char* p)
{
	cs_packet_sign_in*packet= reinterpret_cast<cs_packet_sign_in*>(p);
	db_task dt;
	dt.dt = DB_TASK_TYPE::SIGN_IN;
	dt.obj_id = c_id;
	strcpy_s(dt.user_id, packet->name);
	strcpy_s(dt.user_password, packet->password);
	m_db_queue.push(move(dt));

}

void PacketManager::ProcessSignUp(int c_id, unsigned char* p)
{
	cs_packet_sign_up* packet = reinterpret_cast<cs_packet_sign_up*>(p);
	db_task dt;
	dt.dt = DB_TASK_TYPE::SIGN_UP;
	dt.obj_id = c_id;
	strcpy_s(dt.user_id, packet->name);
	strcpy_s(dt.user_password, packet->password);
	m_db_queue.push(move(dt));
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
	switch (packet->direction)//WORLD크기 정해지면 제한해주기
	{
	case 0://앞
	{
		
		pos += look * MOVE_DISTANCE;
		break;
	}
	case 1://뒤
	{
		pos += look * (MOVE_DISTANCE*-1);
		break;
	}
	case 2://왼
	{
		pos += right * (MOVE_DISTANCE * -1);
		break;
	}
	case 3://오
	{
		pos += right * MOVE_DISTANCE ;
		break;
	}
	}
	cl->SetPosX(pos.x);
	cl->SetPosY(pos.y);
	cl->SetPosZ(pos.z);
	cout << "Packetx :" << pos.x << ", y : " << pos.y << ", z : " << pos.z << endl;
	SendMovePacket(c_id, c_id);
}

void PacketManager::ProcessMatching(int c_id, unsigned char* p)
{
	cs_packet_matching* packet = reinterpret_cast<cs_packet_matching*>(p);
	Player*pl=m_moveobj_manager->GetPlayer(c_id);
	pl->SetMatchUserSize(packet->user_num);
	pl->is_matching = true;
	Player* other_pl = NULL;
	unordered_set<int>match_list;
	//유저 검사 해서 매칭해주는 함수 구현-> 일단 코딩하고 함수화 하자
	match_list.insert(c_id);
	for (int i = 0; i < MAX_USER; ++i)
	{
		other_pl = m_moveobj_manager->GetPlayer(i);
		if (match_list.size() == pl->GetMatchUserSize())
			break;
		if (false == other_pl->is_matching)
			continue;
		if (pl->GetMatchUserSize() != other_pl->GetMatchUserSize())
			continue;
		match_list.insert(i);

	}
	if (match_list.size() == pl->GetMatchUserSize())
	{
		int r_id = m_room_manager->GetEmptyRoom();
		if (-1 == r_id)//빈방 검사
		{
			//방없다고 패킷보내주기
			return;
		}
		
		Room *room=m_room_manager->GetRoom(r_id);
		for (auto id : match_list)
		{
			Player* player = m_moveobj_manager->GetPlayer(id);
			player->is_matching = false;
			player->state_lock.lock();
			player->SetState(STATE::ST_INGAME);
			player->state_lock.unlock();
			room->EnterRoom(id);//방에 아이디 넘겨주기
			cout << id << endl;
			SendMatchingOK(id);
		}
		
		
		
	}
	
	//어차피 다른플레이어가 매칭을 누르지 않으면 기다리는건 롤도 마찬가지
}



void PacketManager::DBThread()
{
	while (true)
	{
		db_task dt;
		if (!m_db_queue.try_pop(dt))
		{
			this_thread::sleep_for(10ms);
			continue;
		}
		ProcessDBTask(dt);
	}
}

void PacketManager::ProcessDBTask(db_task& dt)
{
	LOGINFAIL_TYPE ret=m_db->CheckLoginData(dt.user_id, dt.user_password);
	Player* pl = m_moveobj_manager->GetPlayer(dt.obj_id);
	switch (dt.dt)
	{
	case DB_TASK_TYPE::SIGN_IN:
	{
		if (ret == LOGINFAIL_TYPE::OK)
		{
			//접속꽉찬거는 accept 쪽에서 보내기주기
			pl->state_lock.lock();
			if (STATE::ST_INGAME != pl->GetState())
			{
				pl->SetState(STATE::ST_INGAME);
				pl->state_lock.unlock();
			}
			else
				pl->state_lock.unlock();
			//여기오면 성공패킷 보내주기
		}
		else
		{
			//로그인 실패 패킷보내기
			SendLoginFailPacket(dt.obj_id, static_cast<int>(ret));
		}
		break;
	}
	case DB_TASK_TYPE::SIGN_UP:
	{
		if (ret == LOGINFAIL_TYPE::OK || ret == LOGINFAIL_TYPE::WRONG_PASSWORD)
		{
			m_db2->SaveData(dt.user_id, dt.user_password);
			SendSignUpOK(dt.obj_id);
		}
		else
			// 아이디가 있어 회원가입 불가능 패킷 보내기
			SendLoginFailPacket(dt.obj_id,6);
		break;
	}
	}

}
void PacketManager::JoinDBThread()
{
	db_thread.join();
}
void PacketManager::CreateDBThread()
{

	db_thread=std::thread([this]() {DBThread(); });
}
