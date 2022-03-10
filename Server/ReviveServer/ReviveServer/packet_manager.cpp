#include "pch.h"
#include "packet_manager.h"
#include"db.h"
#include"moveobj_manager.h"
#include"room_manager.h"
#include"room.h"
#include"enemy.h"
using namespace std;

PacketManager::PacketManager()
{
	m_moveobj_manager = new MoveObjManager;
	m_room_manager = new RoomManager;
	m_db = new DB;
	m_db2 = new DB;
}

void PacketManager::Init()
{
	
	m_moveobj_manager->InitPlayer();
	m_moveobj_manager->InitNPC();
	m_room_manager->InitRoom();
	
	m_db->Init();
	
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
	case CS_PACKET_ROTATION: {
		ProcessRotation(c_id, p);
		break;
	}
	}
}

void PacketManager::ProcessAccept(HANDLE hiocp ,SOCKET& s_socket,EXP_OVER*exp_over)
{
	std::cout << "���Ʈó����" << endl;
	SOCKET c_socket = *(reinterpret_cast<SOCKET*>(exp_over->_net_buf));
	int new_id = m_moveobj_manager->GetNewID();
	if (-1 == new_id) {
		std::cout << "Maxmum user overflow. Accept aborted.\n";
		
	}
	else {//�ٽ�����
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




void PacketManager::UpdateObjMove()//�ϴ� ����
{
	for (int i = 0; i < MAX_USER; ++i)
	{
		for (int j = 0; j <= NPC_ID_END; ++j) {
			//���Ŀ� ���� �߰�
			if(i!=j)
			SendMovePacket(i, j);
		}
	}
	SetTimerEvent(0, 0, EVENT_TYPE::EVENT_PLAYER_MOVE, 10);
}

void PacketManager::SpawnEnemy(int room_id)
{
	Room* room = m_room_manager->GetRoom(room_id);
	int curr_round = room->GetRound();
	int sordier_num = room->GetMaxUser() * ( curr_round + 1);
	int king_num = room->GetMaxUser() * curr_round;
	Enemy* enemy = NULL;
	unordered_set<int>enemy_list;
	for (auto e_id : room->GetObjList())
	{
		if (enemy_list.size() == static_cast<INT64>(sordier_num) + king_num)
			break;
		if (true == m_moveobj_manager->IsPlayer(e_id))
			continue;
		Enemy* enemy = m_moveobj_manager->GetEnemy(e_id);
		if (true == enemy->GetIsActive())
			continue;
		if (enemy->GetType() == OBJ_TYPE::OT_NPC_SKULL && enemy_list.size() < sordier_num)
		{
			enemy->SetIsActive(true);
			enemy_list.insert(e_id);
			
		}
		else if (enemy->GetType() == OBJ_TYPE::OT_NPC_SKULLKING) {
			enemy->SetIsActive(true);
			enemy_list.insert(e_id);
		}

	}
	int spawn_idx = rand() % 2;
	float x = ENEMY_SPAWN_POINT[spawn_idx][0];
	float z = ENEMY_SPAWN_POINT[spawn_idx][1];

	for (int i = 0; i < room->GetMaxUser(); ++i)
	{
		for (auto& en : enemy_list)
		{
			enemy = m_moveobj_manager->GetEnemy(en);
			enemy->SetSpawnPoint(x + rand() % 300, z + rand() % 300);
			SendObjInfo(room->GetObjList()[i], en);
			m_timer_queue.push(SetTimerEvent(en, en,room_id, EVENT_TYPE::EVENT_NPC_MOVE, 1));
		}
	}
	cout << "round" << curr_round << "Wave Start" << endl;
	//���⼭ �ѹ��� Ÿ�̸� �̺�Ʈ �־��ֱ�
}

//16ms=60������
void PacketManager::DoEnemyMove(int room_id, int enemy_id)
{
	Room* room = m_room_manager->GetRoom(room_id);
	Enemy* enemy = m_moveobj_manager->GetEnemy(enemy_id);
	//���⺤��,�̵���� ���ֱ�
	//�浹üũ,A*�����ϱ�
	Vector3 nlook;
	Vector3 npos = enemy->GetPos();
	if (enemy->GetTargetId() == -1)//-1���� ���̵�
	{
		Vector3 nlook = Vector3{ BASE_POINT - npos }.Normalrize();
		//�� look���� ����
	}
	else
	{
		//Vector3 nlook = Ÿ�� ������Ʈ - �ڱ� normalize
		Vector3 nlook = Vector3{ m_moveobj_manager->GetPlayer(enemy->GetTargetId())->GetPos() - npos }.Normalrize();
	}
	//Ÿ���� �������� ���� ��� �ٸ��� ���ֱ�
	npos+=(nlook* MAX_SPEED);//�̵��Ÿ� 16ms�����ִ°�
	
	//���⼭ �浹Ȯ���� ������ǥ�� ���ְ� a*����ϱ�
	// a*�� ã�� ����� ������ȯ�������� ������ �̵� ���Ĵ� ������
	enemy->SetPos(npos);

	for (int i = 0; i < room->GetMaxUser(); i++)
	{
		SendMovePacket(i, enemy_id);
		if (true == m_moveobj_manager->IsNear(i, enemy_id))//�̰Ŵ� �þ߹����ȿ� �ִ��� Ȯ��
		{
			//���⼭ ������ �÷��̾� �Ÿ� ����
			//�÷��̾ �� ������ target_id �÷��̾��
			//�ƴϸ� ���� �״��
		}
	}

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
	packet.r_x = p->GetRotation().x;
	packet.r_y = p->GetRotation().y;
	packet.r_z = p->GetRotation().z;
	packet.r_w = p->GetRotation().w;
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
	sc_packet_sign_in_ok packet;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_SIGN_IN_OK;
	packet.id = c_id;
	m_moveobj_manager->GetPlayer(c_id)->DoSend(sizeof(packet), &packet);
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

void PacketManager::SendPutObjPacket(int c_id, int obj_id, OBJ_TYPE obj_type)
{
	sc_packet_put_object packet;
	packet.id = obj_id;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_PUT_OBJECT;
	packet.object_type = (char)obj_type;
	packet.x = m_moveobj_manager->GetMoveObj(obj_id)->GetPosX();
	packet.y = m_moveobj_manager->GetMoveObj(obj_id)->GetPosY();
	packet.z = m_moveobj_manager->GetMoveObj(obj_id)->GetPosZ();
	m_moveobj_manager->GetPlayer(c_id)->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendObjInfo(int c_id, int obj_id)
{
	sc_packet_obj_info packet;
	MoveObj *obj= m_moveobj_manager->GetMoveObj(obj_id);
	packet.id = obj_id;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_OBJ_INFO;
	packet.damage = obj->GetDamge();
	packet.maxhp = obj->GetHP();
	strcpy_s(packet.name, obj->GetName());
	packet.object_type = static_cast<char>(obj->GetType());
	packet.x = obj->GetPosX();
	packet.y = obj->GetPosY();
	packet.z = obj->GetPosZ();
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

timer_event PacketManager::SetTimerEvent(int obj_id, int target_id, int room_id, EVENT_TYPE ev, int seconds)
{
	timer_event t;
	t.obj_id = obj_id;
	t.target_id = target_id;
	t.room_id = room_id;
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
	if (m_room_manager)delete m_room_manager;
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
	Vector3 pos{ packet->x,packet->y,packet->z };
	
	//Room* room = m_room_manager->GetRoom(cl->GetRoomID());
	/*switch (packet->direction)//WORLDũ�� �������� �������ֱ�
	{
	case 0://��
	{
		
		pos += look * MOVE_DISTANCE;
		break;
	}
	case 1://��
	{
		pos += look * (MOVE_DISTANCE*-1);
		break;
	}
	case 2://��
	{
		pos += right * (MOVE_DISTANCE * -1);
		break;
	}
	case 3://��
	{
		pos += right * MOVE_DISTANCE ;
		break;
	}
	}*/

	//cl->SetPos(pos);
	std::cout << "Packet x :" << pos.x << ", y : " << pos.y << ", z : " << pos.z << endl;
	std::cout << "Rotation x :" << packet->r_x << ", y : " << packet->r_y << ", z : " 
		<< packet->r_z<< ", w : " << packet->r_w << endl;
	//for (auto other_pl : room->GetObjList())
	//{
	//	if (false == m_moveobj_manager->IsPlayer(other_pl))
	//		continue;
	//	if (c_id == other_pl)
	//		continue;
	//	SendMovePacket(other_pl, c_id);
	//}
	
}

void PacketManager::ProcessMatching(int c_id, unsigned char* p)
{
	cs_packet_matching* packet = reinterpret_cast<cs_packet_matching*>(p);
	Player*pl=m_moveobj_manager->GetPlayer(c_id);
	pl->SetMatchUserSize(packet->user_num);
	pl->is_matching = true;
	Player* other_pl = NULL;
	vector<int>match_list;
	//���� �˻� �ؼ� ��Ī���ִ� �Լ� ����-> �ϴ� �ڵ��ϰ� �Լ�ȭ ����
	//match_list.insert(c_id);
	for (int i = 0; i < MAX_USER; ++i)
	{
		other_pl = m_moveobj_manager->GetPlayer(i);
		if (match_list.size() == pl->GetMatchUserSize())
			break;
		if (false == other_pl->is_matching)
			continue;
		if (pl->GetMatchUserSize() != other_pl->GetMatchUserSize())
			continue;
		match_list.push_back(i);

	}
	if (match_list.size() == pl->GetMatchUserSize())
	{
		int r_id = m_room_manager->GetEmptyRoom();
		if (-1 == r_id)//��� �˻�
		{
			//����ٰ� ��Ŷ�����ֱ�
			return;
		}
		
		Room *room=m_room_manager->GetRoom(r_id);
		room->Init(pl->GetMatchUserSize());
		for (auto id : match_list)
		{
			Player* player = m_moveobj_manager->GetPlayer(id);
			player->SetRoomID(r_id);
			player->is_matching = false;
			player->state_lock.lock();
			player->SetState(STATE::ST_INGAME);
			player->state_lock.unlock();
			//room->EnterRoom(id);//�濡 ���̵� �Ѱ��ֱ�
			//cout << id << endl;
			SendMatchingOK(id);
		}
		
		room->Init(pl->GetMatchUserSize());
		//�� npc �˻�
		Enemy* e = NULL;
		for (int i = NPC_ID_START; i <= NPC_ID_END; ++i)
		{
			if (match_list.size()-pl->GetMatchUserSize() == room->GetMaxEnemy())
				break;
	
			e=m_moveobj_manager->GetEnemy(i);
			
			if (!e->in_use)
			{
				e->in_use = true;
				e->SetRoomID(r_id);
				match_list.push_back(e->GetID());
			}
			
		}
		//npc���̵� �־��ֱ�
		for (auto obj_id : match_list)
			room->EnterRoom(obj_id);
		
		StartGame(room->GetRoomID());
	}
	
	//������ �ٸ��÷��̾ ��Ī�� ������ ������ ��ٸ��°� �ѵ� ��������
}

void PacketManager::ProcessRotation(int c_id, unsigned char* p)
{
	cs_packet_rotation* packet = reinterpret_cast<cs_packet_rotation*>(p);
	

}

void PacketManager::StartGame(int room_id)
{
	Room*room=m_room_manager->GetRoom(room_id);
	//�� ������Ʈ ������ ������ �ʿ����
	//npc�� player �ʱ�ȭ �� �����ֱ�
	
	Enemy* e = NULL;
	Player* pl = NULL;
	Vector3 pos = Vector3(0.0f, 0.0f, 0.0f);//npc �ʱ�ȭ�� ��ġ ���ļ���
	vector<int>obj_list{ room->GetObjList().begin(),room->GetObjList().end() };
	for (int i=0; i<obj_list.size(); ++i )
	{
		if (i<room->GetMaxUser())
		{
			pl = m_moveobj_manager->GetPlayer(obj_list[i]);
			pl->SetPos(PLAYER_SPAWN_POINT[i]);
			continue;
		}
		e = m_moveobj_manager->GetEnemy(obj_list[i]);
		if (i<room->GetMaxUser() * SORDIER_PER_USER)
		{
			
			e->InitEnemy(OBJ_TYPE::OT_NPC_SKULL, room->GetRoomID(), 
				SKULL_HP, pos, PLAYER_DAMAGE,"Skull Soldier");
			m_moveobj_manager->InitLua("enemy_sordier.lua",e->GetID());
			
		}
		else
		{
			e->InitEnemy(OBJ_TYPE::OT_NPC_SKULLKING, room->GetRoomID(), 
				SKULLKING_HP, pos, PLAYER_DAMAGE, "Skull King");
			m_moveobj_manager->InitLua("enemy_king.lua",e->GetID());
		}
	}

	
	//������ü ���� �����ֱ�� event�� 
	//�÷��̾�� �÷��̾� �����ֱ�
	for (auto c_id : room->GetObjList())
	{
		if (false == m_moveobj_manager->IsPlayer(c_id))
			continue;
	
		pl = m_moveobj_manager->GetPlayer(c_id);
		SendObjInfo(c_id, c_id);//�ڱ��ڽ�
		for (int i=0; i<room->GetMaxUser(); ++i)
		{
			if (c_id == room->GetObjList()[i])continue;
			SendObjInfo(c_id, room->GetObjList()[i]);
		}
	}
	//�� ���Ŀ� npc�� ��� ������ ���ϰ� �̺�Ʈ�� �ѱ�� �ʱ�ȭ -> ȸ�� �ʿ�
	m_timer_queue.push( SetTimerEvent(room->GetRoomID(), 
		room->GetRoomID(), EVENT_TYPE::EVENT_NPC_SPAWN, 3000));
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
	LOGINFAIL_TYPE ret;//m_db->CheckLoginData(dt.user_id, dt.user_password);
	Player* pl = m_moveobj_manager->GetPlayer(dt.obj_id);
	switch (dt.dt)
	{
	case DB_TASK_TYPE::SIGN_IN:
	{
		ret = m_db->CheckLoginData(dt.user_id, dt.user_password);
		if (ret == LOGINFAIL_TYPE::OK)
		{
			//���Ӳ����Ŵ� accept �ʿ��� �������ֱ�
			pl->state_lock.lock();
			if (STATE::ST_INGAME != pl->GetState())
			{
				pl->SetState(STATE::ST_INGAME);
				pl->state_lock.unlock();
			}
			else
				pl->state_lock.unlock();
			strcpy_s(pl->GetName(),MAX_NAME_SIZE ,dt.user_id);
			strcpy_s(pl->GetPassword(), MAX_NAME_SIZE, dt.user_password);
			//������� ������Ŷ �����ֱ�
			SendSignInOK(pl->GetID());
		}
		else
		{
			//�α��� ���� ��Ŷ������
			SendLoginFailPacket(dt.obj_id, static_cast<int>(ret));
		}
		break;
	}
	case DB_TASK_TYPE::SIGN_UP:
	{
		ret = m_db->CheckLoginData(dt.user_id, dt.user_password);
		if (ret == LOGINFAIL_TYPE::NO_ID || ret == LOGINFAIL_TYPE::WRONG_PASSWORD)
		{
			m_db2->SaveData(dt.user_id, dt.user_password);
			SendSignUpOK(dt.obj_id);
		}
		else
			// ���̵� �־� ȸ������ �Ұ��� ��Ŷ ������
			SendLoginFailPacket(dt.obj_id,6);
		break;
	}
	}

}
void PacketManager::JoinDBThread()
{
	db_thread.join();
}

void PacketManager::ProcessTimer(HANDLE hiocp)
{
	while (true) {
		while (true) {
			timer_event ev;
			if (!m_timer_queue.try_pop(ev))continue;
			auto start_t = chrono::system_clock::now();
			if (ev.start_time <= start_t) {
				ProcessEvent(hiocp,ev);
			}
			else {
				m_timer_queue.push(ev);
				break;
			}
		}

		this_thread::sleep_for(10ms);
	}
}

void PacketManager::ProcessEvent(HANDLE hiocp,timer_event& ev)
{
	EXP_OVER* ex_over = new EXP_OVER;
	switch (ev.ev) {
	case EVENT_TYPE::EVENT_NPC_SPAWN:
	{
		ex_over->_comp_op = COMP_OP::OP_NPC_SPAWN;
		ex_over->target_id = ev.target_id;
		break;
	}
	case EVENT_TYPE::EVENT_PLAYER_MOVE: {
		ex_over->_comp_op = COMP_OP::OP_PLAYER_MOVE;
		break;
	}
	case EVENT_TYPE::EVENT_NPC_MOVE: {
		ex_over->_comp_op = COMP_OP::OP_NPC_MOVE;
		ex_over->room_id = ev.room_id;
		ex_over->target_id = ev.target_id;
	}
	}

	PostQueuedCompletionStatus(hiocp, 1, ev.obj_id, &ex_over->_wsa_over);
}
void PacketManager::CreateDBThread()
{

	db_thread=std::thread([this]() {DBThread(); });
}
