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
	MoveObjManager::GetInst();
	m_room_manager = new RoomManager;
	m_db = new DB;
	m_db2 = new DB;
}

void PacketManager::Init()
{
	
	MoveObjManager::GetInst()->InitPlayer();
	MoveObjManager::GetInst()->InitNPC();
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
	//case CS_PACKET_ROTATION: {
	//	ProcessRotation(c_id, p);
	//	break;
	//}
	}
}

void PacketManager::ProcessAccept(HANDLE hiocp ,SOCKET& s_socket,EXP_OVER*exp_over)
{
	std::cout << "억셉트처리옴" << endl;
	SOCKET c_socket = *(reinterpret_cast<SOCKET*>(exp_over->_net_buf));
	int new_id = MoveObjManager::GetInst()->GetNewID();
	if (-1 == new_id) {
		std::cout << "Maxmum user overflow. Accept aborted.\n";
		SendLoginFailPacket(c_socket, static_cast<int>(LOGINFAIL_TYPE::FULL));
	}
	else {//다시제작
		Player* cl = MoveObjManager::GetInst()->GetPlayer(new_id);
		cl->SetID(new_id);
		cl->Init(c_socket);
		CreateIoCompletionPort(reinterpret_cast<HANDLE>(c_socket), hiocp, new_id, 0);
		cl->DoRecv();
		//m_timer_queue.push(SetTimerEvent(new_id, new_id,
		//	EVENT_TYPE::EVENT_TIME, 1000));
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
		MoveObjManager::GetInst()->Disconnect(c_id);
		
	}
	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);
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




void PacketManager::UpdateObjMove()//일단 보류
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
		if (true == MoveObjManager::GetInst()->IsPlayer(e_id))
			continue;
		Enemy* enemy = MoveObjManager::GetInst()->GetEnemy(e_id);
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
			enemy = MoveObjManager::GetInst()->GetEnemy(en);
			enemy->SetSpawnPoint(x+rand()%3000 ,z + rand() % 3000);
			SendObjInfo(room->GetObjList().at(i), en);
			m_timer_queue.push(SetTimerEvent(en, en,room_id, EVENT_TYPE::EVENT_NPC_MOVE, 16));

		}
	}
	cout << "round" << curr_round << "Wave Start" << endl;
	//여기서 한번더 타이머 이벤트 넣어주기
}

//16ms=60프레임
void PacketManager::DoEnemyMove(int room_id, int enemy_id)
{
	Room* room = m_room_manager->GetRoom(room_id);
	Enemy* enemy = MoveObjManager::GetInst()->GetEnemy(enemy_id);
	//방향벡터,이동계산 해주기
	//충돌체크,A*적용하기
	Vector3 nlook;
	Vector3& npos = enemy->GetPos();
	if (enemy->GetTargetId() == -1)//-1기지 아이디
	{
		nlook= Vector3{ BASE_POINT - npos };
		//적 look벡터 설정
	}
	else
	{
		//Vector3 nlook = 타겟 오브젝트 - 자기 normalize
		 nlook= Vector3{ MoveObjManager::GetInst()->GetPlayer(enemy->GetTargetId())->GetPos() - npos };
	}
	//타겟이 누군지에 따라서 계산 다르게 해주기
	Vector3 move_vec=nlook.Normalrize();
	npos+=(move_vec * MAX_SPEED);//이동거리 16ms보내주는거
	cout << move_vec.x << move_vec.y << move_vec.z << endl;
	//여기서 충돌확인후 원래좌표로 해주고 a*사용하기
	// a*로 찾은 경로중 방향전환점까지는 무조건 이동 그후는 버리기
	enemy->SetPos(npos);
	//if(enemy_id==10)
	//	cout << "id" << enemy_id << "pos x:" << npos.x << ", y:" << npos.y << ", z:" << npos.z<<endl;
	for (int i = 0; i < room->GetMaxUser(); i++)
	{
		SendMovePacket(i, enemy_id);
		if (true == MoveObjManager::GetInst()->IsNear(i, enemy_id))//이거는 시야범위안에 있는지 확인
		{
			//여기서 기지와 플레이어 거리 비교후
			//플레이어가 더 가까우면 target_id 플레이어로
			//아니면 기지 그대로
		}
	}
	m_timer_queue.push(SetTimerEvent(enemy_id, enemy_id, room_id, EVENT_TYPE::EVENT_NPC_MOVE, 16));
}




void PacketManager::SendMovePacket(int c_id, int mover)
{
	sc_packet_move packet;
	MoveObj* p = MoveObjManager::GetInst()->GetMoveObj(mover);
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
	MoveObjManager::GetInst()->GetPlayer(c_id)->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendLoginFailPacket(int c_id, int reason)
{
	sc_packet_login_fail packet;
	Player* pl = MoveObjManager::GetInst()->GetPlayer(c_id);
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_LOGIN_FAIL;
	packet.reason = reason;
	pl->DoSend(sizeof(packet), &packet);

}

void PacketManager::SendLoginFailPacket(SOCKET& c_socket, int reason)
{
	sc_packet_login_fail packet;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_LOGIN_FAIL;
	packet.reason = reason;
	EXP_OVER* ex_over = new EXP_OVER(COMP_OP::OP_SEND, sizeof(packet), &packet);
	int ret = WSASend(c_socket, &ex_over->_wsa_buf, 1, 0, 0, &ex_over->_wsa_over, NULL);
}

void PacketManager::SendSignInOK(int c_id)
{
	sc_packet_sign_in_ok packet;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_SIGN_IN_OK;
	packet.id = c_id;
	MoveObjManager::GetInst()->GetPlayer(c_id)->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendSignUpOK(int c_id)
{
	sc_packet_sign_up_ok packet;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_SIGN_UP_OK;
	MoveObjManager::GetInst()->GetPlayer(c_id)->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendMatchingOK(int c_id)
{
	sc_packet_matching packet;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_MATCHING;
	MoveObjManager::GetInst()->GetPlayer(c_id)->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendPutObjPacket(int c_id, int obj_id, OBJ_TYPE obj_type)
{
	sc_packet_put_object packet;
	packet.id = obj_id;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_PUT_OBJECT;
	packet.object_type = (char)obj_type;
	packet.x = MoveObjManager::GetInst()->GetMoveObj(obj_id)->GetPosX();
	packet.y = MoveObjManager::GetInst()->GetMoveObj(obj_id)->GetPosY();
	packet.z = MoveObjManager::GetInst()->GetMoveObj(obj_id)->GetPosZ();
	MoveObjManager::GetInst()->GetPlayer(c_id)->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendObjInfo(int c_id, int obj_id)
{
	sc_packet_obj_info packet;
	MoveObj *obj= MoveObjManager::GetInst()->GetMoveObj(obj_id);
	packet.id = obj_id;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_OBJ_INFO;
	packet.damage = obj->GetDamge();
	packet.maxhp = obj->GetHP();
	strcpy_s(packet.name,MAX_NAME_SIZE+1 ,obj->GetName());
	packet.object_type = static_cast<char>(obj->GetType());
	packet.x = obj->GetPosX();
	packet.y = obj->GetPosY();
	packet.z = obj->GetPosZ();
	MoveObjManager::GetInst()->GetPlayer(c_id)->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendTime(int c_id, float round_time)
{
	sc_packet_time packet;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_TIME;
	packet.time = round_time;
	
	MoveObjManager::GetInst()->GetPlayer(c_id)->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendTestPacket(int c_id, int obj_id, float x, float y, float z)
{
	sc_packet_test packet;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_TEST;
	packet.obj_id = obj_id;
	packet.x = x;
	packet.y = y;
	packet.z = z;
	MoveObjManager::GetInst()->GetPlayer(c_id)->DoSend(sizeof(packet), &packet);
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
	MoveObjManager::GetInst()->DestroyObject();
	if (MoveObjManager::GetInst())delete MoveObjManager::GetInst();
	if (m_db)delete m_db;
	if (m_db2)delete m_db2;
	if (m_room_manager)delete m_room_manager;
}

void PacketManager::Disconnect(int c_id)
{
	MoveObjManager::GetInst()->Disconnect(c_id);
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
	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);
	Vector3 pos{ packet->x,packet->y,packet->z };
	
	Room* room = m_room_manager->GetRoom(cl->GetRoomID());
	/*switch (packet->direction)//WORLD크기 정해지면 제한해주기
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
	}*/

	cl->SetPos(pos);
	std::cout << "Packet x :" << pos.x << ", y : " << pos.y << ", z : " << pos.z << endl;
	std::cout << "Rotation x :" << packet->r_x << ", y : " << packet->r_y << ", z : " 
		<< packet->r_z<< ", w : " << packet->r_w << endl;
	for (auto other_pl : room->GetObjList())
	{
		if (false == MoveObjManager::GetInst()->IsPlayer(other_pl))
			break;
		if (c_id == other_pl)
			continue;
		SendMovePacket(other_pl, c_id);
	}
	
}

void PacketManager::ProcessMatching(int c_id, unsigned char* p)
{
	cs_packet_matching* packet = reinterpret_cast<cs_packet_matching*>(p);
	Player*pl=MoveObjManager::GetInst()->GetPlayer(c_id);
	pl->SetMatchUserSize(packet->user_num);
	pl->is_matching = true;
	Player* other_pl = NULL;
	vector<int>match_list;
	//유저 검사 해서 매칭해주는 함수 구현-> 일단 코딩하고 함수화 하자
	//match_list.insert(c_id);
	for (int i = 0; i < MAX_USER; ++i)
	{
		other_pl = MoveObjManager::GetInst()->GetPlayer(i);
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
		if (-1 == r_id)//빈방 검사
		{
			//방없다고 패킷보내주기
			return;
		}
		
		Room *room=m_room_manager->GetRoom(r_id);
		room->Init(pl->GetMatchUserSize());
		for (auto id : match_list)
		{
			Player* player = MoveObjManager::GetInst()->GetPlayer(id);
			player->SetRoomID(r_id);
			player->is_matching = false;
			player->state_lock.lock();
			player->SetState(STATE::ST_INGAME);
			player->state_lock.unlock();
			//room->EnterRoom(id);//방에 아이디 넘겨주기
			//cout << id << endl;
			SendMatchingOK(id);
		}
		
		room->Init(pl->GetMatchUserSize());
		//빈 npc 검사
		Enemy* e = NULL;
		for (int i = NPC_ID_START; i <= NPC_ID_END; ++i)
		{
			if (match_list.size()-pl->GetMatchUserSize() == room->GetMaxEnemy())
				break;
	
			e=MoveObjManager::GetInst()->GetEnemy(i);
			
			if (!e->in_use)
			{
				e->in_use = true;
				e->SetRoomID(r_id);
				match_list.push_back(e->GetID());
			}
			
		}
		//npc아이디 넣어주기
		for (auto obj_id : match_list)
			room->EnterRoom(obj_id);
		
		StartGame(room->GetRoomID());
	}
	
	//어차피 다른플레이어가 매칭을 누르지 않으면 기다리는건 롤도 마찬가지
}


void PacketManager::StartGame(int room_id)
{
	Room*room=m_room_manager->GetRoom(room_id);
	//맵 오브젝트 정보는 보내줄 필요없음
	//npc와 player 초기화 및 보내주기
	
	Enemy* e = NULL;
	Player* pl = NULL;
	Vector3 pos = Vector3(0.0f, 0.0f, 0.0f);//npc 초기화용 위치 추후수정
	vector<int>obj_list{ room->GetObjList().begin(),room->GetObjList().end() };
	for (int i=0; i<obj_list.size(); ++i )
	{
		if (i<room->GetMaxUser())
		{
			pl = MoveObjManager::GetInst()->GetPlayer(obj_list[i]);
			pl->SetPos(PLAYER_SPAWN_POINT[i]);
			continue;
		}
		e = MoveObjManager::GetInst()->GetEnemy(obj_list[i]);
		if (i<room->GetMaxUser() * SORDIER_PER_USER)
		{
			
			e->InitEnemy(OBJ_TYPE::OT_NPC_SKULL, room->GetRoomID(), 
				SKULL_HP, pos, PLAYER_DAMAGE,"Skull Soldier");
			MoveObjManager::GetInst()->InitLua("enemy_sordier.lua",e->GetID());
			e->lua_lock.lock();
			lua_State* L = e->GetLua();
			lua_getglobal(L, "event_test");
			lua_pushnumber(L, e->GetID());
			int error_num = lua_pcall(L, 1, 0, 0);
			if(error_num)
				MoveObjManager::LuaErrorDisplay(L, error_num);
			e->lua_lock.unlock();
		}
		else
		{
			e->InitEnemy(OBJ_TYPE::OT_NPC_SKULLKING, room->GetRoomID(), 
				SKULLKING_HP, pos, PLAYER_DAMAGE, "Skull King");
			MoveObjManager::GetInst()->InitLua("enemy_king.lua",e->GetID());
			e->lua_lock.lock();
			lua_State* L = e->GetLua();
			lua_getglobal(L, "event_test");
			lua_pushnumber(L,e->GetID());
			cout << "루아에 넣어주는 npc_id:" << e->GetID() << endl;
			int error_num=lua_pcall(L, 1, 0, 0);
			if(error_num)
				MoveObjManager::LuaErrorDisplay(L, error_num);
			e->lua_lock.unlock();
		}
	}

	
	//주위객체 정보 보내주기는 event로 
	//플레이어에게 플레이어 보내주기
	for (auto c_id : room->GetObjList())
	{
		if (false == MoveObjManager::GetInst()->IsPlayer(c_id))
			continue;
	
		pl = MoveObjManager::GetInst()->GetPlayer(c_id);
		cout << "SendObj 이름:" << pl->GetName() << endl;
		SendObjInfo(c_id, c_id);//자기자신
		for (int i=0; i<room->GetMaxUser(); ++i)
		{
			if (c_id == room->GetObjList().at(i))continue;
			SendObjInfo(c_id, room->GetObjList().at(i));
			cout << "안에 SendObj 이름:" << pl->GetName() << endl;
		}
	}
	//몇 초후에 npc를 어디에 놓을지 정하고 이벤트로 넘기고 초기화 -> 회의 필요
	m_timer_queue.push( SetTimerEvent(room->GetRoomID(), 
		room->GetRoomID(), EVENT_TYPE::EVENT_NPC_SPAWN, 10000));//30초다되면 넣어주는걸로 수정?
	//m_timer_queue.push(SetTimerEvent(room->GetRoomID(), room->GetRoomID(),
	//	EVENT_TYPE::EVENT_TIME, 1000));
	
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
	Player* pl = MoveObjManager::GetInst()->GetPlayer(dt.obj_id);
	switch (dt.dt)
	{
	case DB_TASK_TYPE::SIGN_IN:
	{
		for (int i = 0; i < MAX_USER; ++i)
		{
			Player*other_pl = MoveObjManager::GetInst()->GetPlayer(i);
			other_pl->state_lock.lock();
			if (other_pl->GetState() == STATE::ST_FREE)
			{
				other_pl->state_lock.unlock();
				continue;
			}
			other_pl->state_lock.unlock();
			if (strcmp(other_pl->GetName(), dt.user_id) == 0)
			{
				ret = LOGINFAIL_TYPE::AREADY_SIGHN_IN;
				SendLoginFailPacket(dt.obj_id, (int)ret);
				return;
			}
		}
		ret = m_db->CheckLoginData(dt.user_id, dt.user_password);
		if (ret == LOGINFAIL_TYPE::OK)
		{
			//이미 접속해있는지 확인
			
			//접속꽉찬거는 accept 쪽에서 보내기주기
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
			cout << "이름 : " << pl->GetName() << "비번 : " << pl->GetPassword();
			//여기오면 성공패킷 보내주기
			SendSignInOK(pl->GetID());
		}
		else
		{
			//로그인 실패 패킷보내기
			SendLoginFailPacket(dt.obj_id, static_cast<int>(ret));
		}
		//test용 코드 이후에 꼭 지우자
		//for (int i = 0; i < 10; ++i)
		//{
		//	SendTestPacket(pl->GetID(), i * 20, i * 100.0f, 0, i * 100.0f);
		//}
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
			else if (10ms >= ev.start_time - start_t)
			{
				this_thread::sleep_for(ev.start_time - start_t);
				ProcessEvent(hiocp, ev);
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
		PostQueuedCompletionStatus(hiocp, 1, ev.obj_id, &ex_over->_wsa_over);
		break;
	}
	case EVENT_TYPE::EVENT_PLAYER_MOVE: {
		ex_over->_comp_op = COMP_OP::OP_PLAYER_MOVE;
		PostQueuedCompletionStatus(hiocp, 1, ev.obj_id, &ex_over->_wsa_over);
		break;
	}
	case EVENT_TYPE::EVENT_NPC_MOVE: {
		ex_over->_comp_op = COMP_OP::OP_NPC_MOVE;
		ex_over->room_id = ev.room_id;
		ex_over->target_id = ev.target_id;
		PostQueuedCompletionStatus(hiocp, 1, ev.obj_id, &ex_over->_wsa_over);
		break;
	}
	case EVENT_TYPE::EVENT_TIME: {
		//방으로 처리하도록 바꾸기
		Room*room=m_room_manager->GetRoom(ev.target_id);
		auto end_time = std::chrono::system_clock::now()+1000ms;
		std::chrono::duration<float> elapsed = end_time - ev.start_time;
		room->SetRoundTime(room->GetRoundTime() - elapsed.count());
		//ro_time -= elapsed.count();
		//cout << "duration:" << elapsed.count() << endl;
		for (int pl : room->GetObjList())
		{
			if (false == MoveObjManager::GetInst()->IsPlayer(pl))
				break;
			SendTime(pl, room->GetRoundTime());
		}
		if (0.01f >= room->GetRoundTime())
		{
			room->SetRoundTime(30.0f);
			room->SetRound(room->GetRound() + 1);
			m_timer_queue.push(SetTimerEvent(room->GetRoomID(),
				room->GetRoomID(), EVENT_TYPE::EVENT_NPC_SPAWN, 16));
		}
		m_timer_queue.push(SetTimerEvent(ev.obj_id, ev.target_id,
			EVENT_TYPE::EVENT_TIME, 1000));
		break;
	}
	}

	
}
void PacketManager::CreateDBThread()
{

	db_thread=std::thread([this]() {DBThread(); });
}
