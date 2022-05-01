#include<map>
#include<set>
#include<bitset>
#include "pch.h"
#include "packet_manager.h"
#include"database/db.h"
#include"object/moveobj_manager.h"
#include"room/room_manager.h"
#include"room/room.h"
#include"object/move_objects/enemy.h"
#include"map/map_manager.h"
#include"lua/functions/lua_functions.h"
#include"util/Astar.h"
#include"util/collision/collisioner.h"
#include"util/collision/collision_checker.h"
#include"object/bullet.h"
concurrency::concurrent_priority_queue<timer_event> PacketManager::g_timer_queue = concurrency::concurrent_priority_queue<timer_event>();
//#include"map_loader.h"
const int ROUND_TIME = 10000;
using namespace std;
PacketManager::PacketManager()
{
	MoveObjManager::GetInst();
	m_room_manager = new RoomManager;
	m_map_manager = new MapManager;
	//m_move_obj_manager = new MoveObjManager;
	//m_map_manager->LoadMap("map.txt");
	m_db = new DB;
	m_db2 = new DB;
	
}

void PacketManager::Init()
{
	
	MoveObjManager::GetInst()->InitPlayer();
	MoveObjManager::GetInst()->InitNPC();
	m_room_manager->InitRoom();
	m_map_manager->LoadMap("src/map/map.txt");
	m_db->Init();
	
	m_db2->Init();
	//Astar astar;
	//astar.SearchAllPath(m_map_manager->GetMapObjVec(), Vector3(2300.f, 0.f, 2700.f), Vector3(2400.f, 0.f, 3400.f));
	//cout << "Astar size" << sizeof(astar) << endl;
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
	case CS_PACKET_HIT: {
		ProcessHit(c_id, p);
		break;
	}
	case CS_PACKET_GAME_START: {
		ProcessGameStart(c_id, p);
		break;
	}
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
	else {
		Player* cl = MoveObjManager::GetInst()->GetPlayer(new_id);
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
		
		if (true == enemy->in_game)
			continue;
		if (enemy->GetType() == OBJ_TYPE::OT_NPC_SKULL && enemy_list.size() < sordier_num)
		{
			enemy->in_game = true;
			enemy->SetIsActive(true);
			
			enemy_list.insert(e_id);
			
		}
		else if (enemy->GetType() == OBJ_TYPE::OT_NPC_SKULLKING) {
			enemy->in_game = true;
			enemy->SetIsActive(true);
			enemy_list.insert(e_id);
		}
	
	}
	
	//cout << "round" << room->GetRound() << endl;
	//cout <<"e_list 사이즈:" << enemy_list.size() << endl;
	//for (auto& id : enemy_list)
	//{
	//	cout << "현재 적 id" << id << " ";
	//}
	//cout << endl;
	if (enemy_list.size() < static_cast<INT64>(sordier_num) + king_num)
		cout << "적 객체가 모자랍니다" << endl;
	vector<MapObj>spawn_area;
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> random_point(0, 1);
	spawn_area.reserve(10);
	//for (auto a : m_map_manager->GetMapObjVec())
	//{
	//	if (OBJ_TYPE::OT_SPAWN_AREA != a.GetType())continue;
	//	spawn_area.push_back(a);
	//}
	
	//for (auto& en : enemy_list)
	//{
	//	int spawn_idx = random_point(gen);
	//	uniform_int_distribution<int> random_pos_x(static_cast<int>(spawn_area[spawn_idx].GetPosX() - spawn_area[spawn_idx].GetExtent().x),
	//		static_cast<int>(spawn_area[spawn_idx].GetPosX() + spawn_area[spawn_idx].GetExtent().x));
	//
	//	uniform_int_distribution<int> random_pos_z(static_cast<int>(spawn_area[spawn_idx].GetPosZ() - spawn_area[spawn_idx].GetExtent().z),
	//		static_cast<int>(spawn_area[spawn_idx].GetPosZ() + spawn_area[spawn_idx].GetExtent().z));
	//	enemy = MoveObjManager::GetInst()->GetEnemy(en);
	//	enemy->SetSpawnPoint(random_pos_x(gen), random_pos_z(gen));
	//	enemy->GetCollision().UpdateCollision(enemy->GetPos());
	//}
	
	int i = 0;
	for (auto &en : enemy_list)
	{

		//for (auto pl : room->GetObjList())
		//{
		//	if (false == MoveObjManager::GetInst()->IsPlayer(pl))continue;
		//	SendObjInfo(pl, en);
			g_timer_queue.push(SetTimerEvent(en, en, room_id, EVENT_TYPE::EVENT_NPC_TIMER_SPAWN,(2000*i)+500 ));
			++i;
		//}
		//g_timer_queue.push(SetTimerEvent(en, en, room_id, EVENT_TYPE::EVENT_NPC_MOVE, 30+en*100));
	}
		
		
		//enemy->SetMoveTime(300);
		
	
	
	//cout << "round" << curr_round << "Wave Start" << endl;
	//여기서 한번더 타이머 이벤트 넣어주기
}

void PacketManager::SpawnEnemyByTime(int enemy_id, int room_id)
{
	Room* room = m_room_manager->GetRoom(room_id);
	Enemy* enemy = NULL;
	vector<MapObj>spawn_area;
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> random_point(0, 1);
	spawn_area.reserve(10);

	for (auto a : m_map_manager->GetMapObjVec())
	{
		if (OBJ_TYPE::OT_SPAWN_AREA != a.GetType())continue;
		spawn_area.push_back(a);
	}
	int spawn_idx = random_point(gen);
	uniform_int_distribution<int> random_pos_x(static_cast<int>(spawn_area[spawn_idx].GetPosX() - spawn_area[spawn_idx].GetExtent().x),
		static_cast<int>(spawn_area[spawn_idx].GetPosX() + spawn_area[spawn_idx].GetExtent().x));
	
	uniform_int_distribution<int> random_pos_z(static_cast<int>(spawn_area[spawn_idx].GetPosZ() - spawn_area[spawn_idx].GetExtent().z),
		static_cast<int>(spawn_area[spawn_idx].GetPosZ() + spawn_area[spawn_idx].GetExtent().z));
	enemy = MoveObjManager::GetInst()->GetEnemy(enemy_id);
	enemy->SetSpawnPoint(random_pos_x(gen), random_pos_z(gen));
	enemy->GetCollision().UpdateCollision(enemy->GetPos());
	for (auto pl : room->GetObjList())
	{
		if (false == MoveObjManager::GetInst()->IsPlayer(pl))continue;
		SendObjInfo(pl, enemy_id);
	}	
	g_timer_queue.push(SetTimerEvent(enemy_id, enemy_id, room_id, EVENT_TYPE::EVENT_NPC_MOVE, 30));
}


void PacketManager::DoEnemyMove(int room_id, int enemy_id)
{
	Room* room = m_room_manager->GetRoom(room_id);
	
	Enemy*  enemy = MoveObjManager::GetInst()->GetEnemy(enemy_id);
	if (false == enemy->GetIsActive())return;
	//cout << "enemy_id" << enemy->GetID() << endl;
	//방향벡터,이동계산 해주기
	//충돌체크,A*적용하기
	//Vector3& nlook = enemy->GetLookVec();
	
	const Vector3 base_pos = m_map_manager->GetMapObjectByType(OBJ_TYPE::OT_BASE).GetGroundPos();
	if (enemy->GetTargetId() == -1)//-1기지 아이디
	{
		enemy->DoMove(base_pos);
		//enemy->DoMove(Vector3(base_pos.x, base_pos.y, base_pos.z + 400.0f));
		
		//enemy->GetCollision().UpdateCollision(enemy->GetPos());
		
	}
	else
	{
		enemy->DoMove(MoveObjManager::GetInst()->GetPlayer(enemy->GetTargetId())->GetPos());
		//enemy->GetCollision().UpdateCollision(enemy->GetPos());
	}
	
	if (false == m_map_manager->CheckInRange(enemy->GetCollision()))
	{
		enemy->SetToPrevPos();
		if (enemy->GetTargetId() == -1)
			enemy->DoPrevMove(Vector3(base_pos.x, enemy->GetPrevPos().y, enemy->GetPrevPos().z));
	}
	else
	{
		if (true == m_map_manager->CheckCollision(enemy->GetCollision()))
		{
			unique_ptr<Astar>astar = make_unique<Astar>();
			 //astar_ret = true;
			if (enemy->GetTargetId() != -1) {
				bool astar_ret = astar->SearchAllPath(m_map_manager->GetMapObjVec(), enemy->GetPos(),
					MoveObjManager::GetInst()->GetPlayer(enemy->GetTargetId())->GetPos(), enemy->GetCollision());
				//astar_ret ? cout << "길찾기 성공" : cout << "길찾기 실패";
				//cout << endl;
			}
			//cout << "콜리전은 OK" << endl;
		}
		else
		{
			//Enemy* other_enemy = NULL;
			//for (auto& npc_id : room->GetObjList())
			//{
			//	if (true == MoveObjManager::GetInst()->IsPlayer(npc_id))continue;
			//	if (false == enemy->GetIsActive())continue;
			//	if (enemy_id == npc_id)continue;
			//	other_enemy = MoveObjManager::GetInst()->GetEnemy(npc_id);
			//	if (true == CollisionChecker::CheckCollisions(enemy->GetCollision(), other_enemy->GetCollision()))
			//	{
			//		
			//		enemy->SetToPrevPos();
			//		
			//		g_timer_queue.push(SetTimerEvent(enemy_id, enemy_id, room_id, EVENT_TYPE::EVENT_NPC_MOVE, 50));
			//		cout << "return 직전" << endl;
			//		return;
			//	}
			//}

		}
		
	}
		//A*는 플레이어 쫓을때만 사용 이거는 중간으로 이동후 직진하도록 만듬
		//unique_ptr<Astar>astar=make_unique<Astar>();
		
		//if (enemy->GetTargetId() == -1)
		//{
			//if (enemy->m_load.size() == 0)
			//{
			//	if (astar->SearchAllPath(m_map_manager->GetMapObjVec(), enemy->GetPos(),
			//		Vector3(base.GetPos().x + base.GetExtent().x + 45.0f, 300.0f, base.GetPos().z + base.GetExtent().z + 45.0f)))
			//	{
			//		enemy->m_load.assign(astar->m_enemy_load.begin(), astar->m_enemy_load.end());
			//		nlook = Vector3{ enemy->m_load.front() - curr_pos };
			//		move_vec = nlook.Normalrize();
			//		npos = curr_pos + (move_vec * MAX_SPEED);
			//		enemy->SetPos(npos);
			//		cout << "vector copy" << endl;
			//	}
			//	else
			//	{
			//		nlook = Vector3{ Vector3(base.GetPosX(),curr_pos.y,curr_pos.z) - curr_pos };
			//		move_vec = nlook.Normalrize();
			//		npos = curr_pos + (move_vec * MAX_SPEED);
			//		enemy->SetPos(npos);
			//	}
			//}
			//else
			//{
			//	if (enemy->m_load.front().x <= enemy->GetPosX() && enemy->m_load.front().z <= enemy->GetPosZ())
			//	{
			//		enemy->m_load.erase(enemy->m_load.begin());
			//	}
				//nlook = Vector3{ enemy->m_load.front() - curr_pos };
				//move_vec = nlook.Normalrize();
				//npos = curr_pos + (move_vec * MAX_SPEED);
				//enemy->SetPos(npos);
				//cout << "vector pop" << endl;
			//}
		
		
		
	
		//enemy->DoPrevMove(Vector3(base_pos.x, enemy->GetPrevPos().y, enemy->GetPrevPos().z));

	
	// a*로 찾은 경로중 방향전환점까지는 무조건 이동 그후는 버리기
	//다음 위치 보내주기
	
	

	for (auto pl : room->GetObjList())
	{
		if (false == MoveObjManager::GetInst()->IsPlayer(pl))continue;
		SendMovePacket(pl, enemy_id);
	}
	CallStateMachine(enemy_id, room_id, base_pos);
	//g_timer_queue.push(SetTimerEvent(enemy_id, enemy_id, room_id, EVENT_TYPE::EVENT_NPC_MOVE, 50));
}

void PacketManager::CountTime(int room_id)
{
	Room* room = m_room_manager->GetRoom(room_id);
	auto end_time = std::chrono::system_clock::now();
	std::chrono::duration<float> elapsed = room->GetRoundTime() - end_time;

	for (int pl : room->GetObjList())
	{
		if (false == MoveObjManager::GetInst()->IsPlayer(pl))
			break;
		SendTime(pl, elapsed.count());
	}
	if (end_time >= room->GetRoundTime())
	{
		room->SetRoundTime(ROUND_TIME);
		if (room->GetRound() < 3)
		{
			room->SetRound(room->GetRound() + 1);
			g_timer_queue.push(SetTimerEvent(room->GetRoomID(),
				room->GetRoomID(), EVENT_TYPE::EVENT_NPC_SPAWN, 30));
		}
		
	}
	if (room->GetRound() == 3)
	{
		Enemy* enemy = nullptr;
		int live_count{ 0 };
		for (int obj_id : room->GetObjList())
		{
			if (true == MoveObjManager::GetInst()->IsPlayer(obj_id))continue;
			enemy = MoveObjManager::GetInst()->GetEnemy(obj_id);
			if (enemy->GetIsActive() == true)live_count++;
		}
		if (live_count == 0)
		{
			for (auto p_id : room->GetObjList())
			{
				if (false == MoveObjManager::GetInst()->IsPlayer(p_id))continue;
				SendGameWin(p_id);
			}
		}
	}
	g_timer_queue.push(SetTimerEvent(room_id, room_id,
		EVENT_TYPE::EVENT_TIME, 1000));
}

void PacketManager::DoEnemyAttack(int enemy_id, int target_id, int room_id)
{
	//초당두발
	Room* room = m_room_manager->GetRoom(room_id);
	Enemy* enemy = MoveObjManager::GetInst()->GetEnemy(enemy_id);

	float base_hp=99999.f;
	if (false == enemy->GetIsActive())return;

	if (target_id == -1)
	{
		room->m_base_hp_lock.lock();
		base_hp = room->GetBaseHp()- enemy->GetDamge();
		//room->SetBaseHp(base_hp);
		room->m_base_hp_lock.unlock();
	}
	for (int pl : room->GetObjList())
	{
		if (false==MoveObjManager::GetInst()->IsPlayer(pl))continue;
		if (target_id == -1)
		{
			SendNPCAttackPacket(pl, enemy_id, target_id);
			SendBaseStatus(pl, room_id);
		}
		else
			SendNPCAttackPacket(pl, enemy_id, target_id);
	}
	if (base_hp <= 0.0f)
	{
		for (int pl : room->GetObjList())
		{
			if (false == MoveObjManager::GetInst()->IsPlayer(pl))continue;
			SendGameDefeat(pl);
		}
	}
	auto& attack_time = enemy->GetAttackTime();
	attack_time = chrono::system_clock::now() + 1s;
	const Vector3 base_pos = m_map_manager->GetMapObjectByType(OBJ_TYPE::OT_BASE).GetGroundPos();
	CallStateMachine(enemy_id, room_id, base_pos);
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
	
	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);
	cl->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendNPCAttackPacket(int c_id,int obj_id, int target_id)
{
	sc_packet_npc_attack packet;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_NPC_ATTACK;
	packet.obj_id = obj_id;
	packet.target_id = target_id;
	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);
	cl->DoSend(sizeof(packet), &packet);
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
	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);
	cl->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendTime(int c_id, float round_time)
{
	sc_packet_time packet;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_TIME;
	packet.time = round_time;
	packet.send_time = chrono::system_clock::now().time_since_epoch().count();
	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);
	cl->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendTestPacket(int c_id, int mover, float x, float y, float z)
{
	sc_packet_test packet;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_TEST;
	packet.id = mover;
	packet.x = x;
	packet.y = y;
	packet.z = z;

	MoveObjManager::GetInst()->GetPlayer(c_id)->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendAttackPacket(int c_id, int attacker)
{
	sc_packet_attack packet;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_ATTACK;
	packet.obj_id = attacker;
	MoveObjManager::GetInst()->GetPlayer(c_id)->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendBaseStatus(int c_id ,int room_id)
{
	sc_packet_base_status packet;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_BASE_STATUS;
	packet.hp = m_room_manager->GetRoom(room_id)->GetBaseHp();
	packet.room_id = room_id;
	MoveObjManager::GetInst()->GetPlayer(c_id)->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendStatusChange(int c_id, int obj_id, float hp)
{
	sc_packet_status_change packet;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_STATUS_CHANGE;
	packet.id = obj_id;
	packet.hp = hp;
	MoveObjManager::GetInst()->GetPlayer(c_id)->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendGameWin(int c_id)
{
	sc_packet_win packet;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_WIN;

	MoveObjManager::GetInst()->GetPlayer(c_id)->DoSend(sizeof(packet), &packet);
}


void PacketManager::SendGameDefeat(int c_id)
{

	sc_packet_defeat packet;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_DEFEAT;

	MoveObjManager::GetInst()->GetPlayer(c_id)->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendDead(int c_id, int obj_id)
{
	sc_packet_dead packet;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_DEAD;
	packet.obj_id = obj_id;
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



bool PacketManager::CheckPlayerBullet(int c_id, const Vector3& position, const Vector3& forward)
{
	Player* player = MoveObjManager::GetInst()->GetPlayer(c_id);
	Room* room = m_room_manager->GetRoom(player->GetRoomID());
	Vector3 last_pos = position + (forward * ATTACK_RANGE);
	cout << "라스트 포즈:" << last_pos << endl;
	Vector3 shoot_vec = forward * ATTACK_RANGE;
	Vector2 comp_vec(last_pos.x, last_pos.z);
	Enemy* enemy = NULL;
	map<float, int>result_enemy;
	//unordered_map<int, float>obj_dist;
	map<float, int>result_map;
	//vector<MapObj>map_vec;
	//map_vec.reserve(100);
	Vector2 ret_point = Vector2(0.0f, 0.0f);
	bool bret = false;
	BoxCollision en_col;
	float map_dist = 999999.f;
	for (int e_id : room->GetObjList())
	{
		if (true == MoveObjManager::GetInst()->IsPlayer(e_id))continue;
		enemy = MoveObjManager::GetInst()->GetEnemy(e_id);
		if (false == enemy->GetIsActive())continue;
		//if (position.Dist2d(enemy->GetPos()) > ATTACK_RANGE)continue;
		//if (comp_vec * Vector2(enemy->GetPosX() - position.x,
		//	enemy->GetPosZ() - position.z) < 0)continue;

		map_dist = 999999.f;
		bret = false;

		en_col = enemy->GetCollision();
		BoxCollision2D box_col{ en_col.GetMinPos(),en_col.GetMaxPos() };
		bret = CollisionChecker::segmentIntersection(Vector2(position.x, position.z), comp_vec,
			box_col.p[0], box_col.p[3], ret_point);
		if (bret) {
			result_enemy[ret_point.Dist2d(Vector2(position.x, position.z))] = enemy->GetID();
			map_dist = ret_point.Dist2d(Vector2(position.x, position.z));
		}
		for (int i = 0; i < 3; ++i)
		{
			bret = CollisionChecker::segmentIntersection(Vector2(position.x, position.z), comp_vec,
				box_col.p[i], box_col.p[i + 1], ret_point);
			if (bret && map_dist > ret_point.Dist2d(Vector2(position.x, position.z)))
			{
				result_enemy[ret_point.Dist2d(Vector2(position.x, position.z))] = enemy->GetID();
				map_dist = ret_point.Dist2d(Vector2(position.x, position.z));
			}
		}
	}
	ret_point = Vector2(0.0f, 0.0f);
	map_dist = 999999.f;
	for (MapObj map_obj : m_map_manager->GetMapObjVec())
	{
		if (false == map_obj.GetIsBlocked())continue;
		map_dist = 999999.f;
		BoxCollision2D box_col{ map_obj.GetMinPos(),map_obj.GetMaxPos() };
		bret = CollisionChecker::segmentIntersection(Vector2(position.x, position.z), comp_vec,
			box_col.p[0], box_col.p[3], ret_point);
		if (bret) {
			result_map[ret_point.Dist2d(Vector2(position.x, position.z))] = map_obj.GetID();
			map_dist = ret_point.Dist2d(Vector2(position.x, position.z));
		}
		for (int i = 0; i < 3; ++i)
		{
			bret = CollisionChecker::segmentIntersection(Vector2(position.x, position.z), comp_vec,
				box_col.p[i], box_col.p[i + 1], ret_point);
			if (bret && map_dist > ret_point.Dist2d(Vector2(position.x, position.z)))
			{
				result_map[ret_point.Dist2d(Vector2(position.x, position.z))] = map_obj.GetID();
				map_dist = ret_point.Dist2d(Vector2(position.x, position.z));
			}
		}

	}
	float enemy_min = 99999.f;
	float map_min = 99999.f;
	float final_result = 0;
	if (result_enemy.begin() != result_enemy.end()) enemy_min = result_enemy.begin()->first;
	if (result_map.begin() != result_map.end())  map_min = result_map.begin()->first;
	//if (enemy_min == map_min)return;
	cout << "적 최소:" << enemy_min << "맵 사이즈" << result_enemy.size() << endl;
	cout << "맵 최소:" << map_min << endl;
	int hit_id = -99;
	if (map_min > enemy_min)hit_id = result_enemy[enemy_min];
	else cout << "맵 오브젝트 충돌" << endl;
	if (hit_id == -99)return false;
	enemy = MoveObjManager::GetInst()->GetEnemy(hit_id);
	enemy->SetHP(enemy->GetHP() - player->GetDamge());
	for (int pl : room->GetObjList())
	{
		if (false == MoveObjManager::GetInst()->IsPlayer(pl))continue;

		SendStatusChange(pl, hit_id, enemy->GetHP());
	}
	cout << hit_id << endl;
	return true;
}

void PacketManager::CallStateMachine(int enemy_id, int room_id, const Vector3& base_pos)
{
	Enemy* enemy = MoveObjManager::GetInst()->GetEnemy(enemy_id);
	Room* room = m_room_manager->GetRoom(room_id);
	map<float, int>distance_map;

	float base_dist = sqrt(pow(abs(base_pos.x - enemy->GetPos().x), 2) + pow(abs(base_pos.z - enemy->GetPos().z), 2));
	distance_map.try_emplace(base_dist, -1);
	Player* player = NULL;
	auto check_end_time = std::chrono::system_clock::now();
	auto& check_time = enemy->GetCheckTime();
	int target_id =enemy->GetTargetId();
	if (check_time <= check_end_time) {
		for (auto pl : room->GetObjList())
		{
			if (false == MoveObjManager::GetInst()->IsPlayer(pl))continue;

			if (true == MoveObjManager::GetInst()->IsNear(pl, enemy_id))//이거는 시야범위안에 있는지 확인
			{
				player = MoveObjManager::GetInst()->GetPlayer(pl);
				if (false == m_map_manager->CheckInRange(player->GetPos())) continue;
				auto fail_obj = distance_map.try_emplace(MoveObjManager::GetInst()->ObjDistance(pl, enemy_id), pl);

				//여기서 기지와 플레이어 거리 비교후
				//플레이어가 더 가까우면 target_id 플레이어로
				//아니면 기지 그대로
			}
		}
		auto nealist = distance_map.begin();
		target_id = nealist->second;
		check_time = check_end_time + 1s;
	}


	lua_State* L = enemy->GetLua();
	enemy->lua_lock.lock();
	lua_getglobal(L, "state_machine");
	lua_pushnumber(L, target_id);
	int err = lua_pcall(L, 1, 0, 0);
	if (err)
		MoveObjManager::LuaErrorDisplay(L, err);
	enemy->lua_lock.unlock();
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
	cs_packet_attack* packet = reinterpret_cast<cs_packet_attack*>(p);
	Player* player = MoveObjManager::GetInst()->GetPlayer(c_id);
	if (player->GetRoomID() == -1)return;
	Room* room = m_room_manager->GetRoom(player->GetRoomID());
	Vector3 position{ packet->x,packet->y,packet->z };
	Vector3 forward{ packet->f_x,packet->f_y,packet->f_z };
	//cout << position << endl;
	//cout << forward << endl;
	for (int pl : room->GetObjList())
	{
		if (false == MoveObjManager::GetInst()->IsPlayer(pl))continue;
		if (pl == c_id)continue;
		SendAttackPacket(pl,c_id);
	}
	
	
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
	//std::cout << "Packet x :" << pos.x << ", y : " << pos.y << ", z : " << pos.z << endl;
	//std::cout << "Rotation x :" << packet->r_x << ", y : " << packet->r_y << ", z : " 
	//	<< packet->r_z<< ", w : " << packet->r_w << endl;
	for (auto other_pl : room->GetObjList())
	{
		if (false == MoveObjManager::GetInst()->IsPlayer(other_pl))
			continue;
		//if (c_id == other_pl)
		//	continue;
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
			player->SetRoomID(r_id);
			player->SetIsActive(true);
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
			
			if (false==e->in_use)
			{
				e->in_use = true;
				e->SetRoomID(r_id);
				match_list.push_back(e->GetID());
			}
			
		}
		//npc아이디 넣어주기
		for (auto obj_id : match_list)
			room->EnterRoom(obj_id);
		
		//StartGame(room->GetRoomID());
	}
	
	//어차피 다른플레이어가 매칭을 누르지 않으면 기다리는건 롤도 마찬가지
}

void PacketManager::ProcessHit(int c_id, unsigned char* p)
{
	cs_packet_hit* packet = reinterpret_cast<cs_packet_hit*>(p);
	MoveObj* victim = MoveObjManager::GetInst()->GetMoveObj(packet->victim_id);
	MoveObj*attacker= MoveObjManager::GetInst()->GetMoveObj(packet->attacker_id);
	Room* room = m_room_manager->GetRoom(attacker->GetRoomID());
	float hp;
	if (false == victim->GetIsActive())return;
	victim->m_hp_lock.lock();
	victim->SetHP(victim->GetHP() - attacker->GetDamge());
	hp = victim->GetHP();
	//player였으면 게임오버 추가
	victim->m_hp_lock.unlock();
	for (int obj_id : room->GetObjList())
	{
		if (false == MoveObjManager::GetInst()->IsPlayer(obj_id))continue;
		//if (victim->GetID() == obj_id||attacker->GetID()==obj_id)continue;
		SendStatusChange(obj_id, victim->GetID(), victim->GetHP());
	}
	
	if (hp <= 0.0f)
	{
		victim->SetIsActive(false);
		for (int obj_id : room->GetObjList())
		{
			if (false == MoveObjManager::GetInst()->IsPlayer(obj_id))continue;
			SendDead(obj_id, victim->GetID());
		}
		if (victim->GetType() == OBJ_TYPE::OT_PLAYER)
		{
			for (int obj_id : room->GetObjList())
			{
				if (false == MoveObjManager::GetInst()->IsPlayer(obj_id))continue;
				SendGameDefeat(obj_id);
			}
		}
	}

}

void PacketManager::ProcessGameStart(int c_id, unsigned char* p)
{
	cs_packet_game_start* packet = reinterpret_cast<cs_packet_game_start*>(p);
	Player* player = MoveObjManager::GetInst()->GetPlayer(c_id);
	player->SetIsReady(true);
	Room* room = m_room_manager->GetRoom(player->GetRoomID());
	for (auto pl : room->GetObjList())
	{
		if (false == MoveObjManager::GetInst()->IsPlayer(pl))continue;
		if (false == MoveObjManager::GetInst()->GetPlayer(pl)->GetIsReady())return;
	
	}
		StartGame(room->GetRoomID());
}


void PacketManager::StartGame(int room_id)
{
	Room*room=m_room_manager->GetRoom(room_id);
	//맵 오브젝트 정보는 보내줄 필요없음
	//npc와 player 초기화 및 보내주기
	const Vector3 base_pos = m_map_manager->GetMapObjectByType(OBJ_TYPE::OT_BASE).GetGroundPos();
	Enemy* e = NULL;
	Player* pl = NULL;
	Vector3 pos = Vector3(0.0f, 300.0f, 0.0f);//npc 초기화용 위치 추후수정
	vector<int>obj_list{ room->GetObjList().begin(),room->GetObjList().end() };
	for (int i=0; i<obj_list.size(); ++i )
	{
		if (i<room->GetMaxUser())
		{
			pl = MoveObjManager::GetInst()->GetPlayer(obj_list[i]);
			pl->SetPos(m_map_manager->PLAYER_SPAWN_POINT[i]);
			continue;
		}
		e = MoveObjManager::GetInst()->GetEnemy(obj_list[i]);
		if (i<room->GetMaxUser() * SORDIER_PER_USER)
		{
			
			e->InitEnemy(OBJ_TYPE::OT_NPC_SKULL, room->GetRoomID(), 
				SKULL_HP, pos, PLAYER_DAMAGE,"Skull Soldier");
			MoveObjManager::GetInst()->InitLua("src/lua/sclipt/enemy_sordier.lua",e->GetID(),base_pos);
			e->SetCollision(BoxCollision(pos, SOLDIER_LOCAL_POS, SOLDIER_EXTENT, SOLDIER_SCALE));
			
		}
		else
		{
			e->InitEnemy(OBJ_TYPE::OT_NPC_SKULLKING, room->GetRoomID(), 
				SKULLKING_HP, pos, PLAYER_DAMAGE, "Skull King");
			MoveObjManager::GetInst()->InitLua("src/lua/sclipt/enemy_king.lua",e->GetID(),base_pos);
			e->SetCollision(BoxCollision(pos, KING_LOCAL_POS, KING_EXTENT, KING_SCALE));
			
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
		for (auto other_id : room->GetObjList())
		{
			if (false == MoveObjManager::GetInst()->IsPlayer(other_id))
				continue;
			if (c_id == other_id)continue;
			SendObjInfo(c_id, other_id);
			cout << "안에 SendObj 이름:" << pl->GetName() << endl;
		}
		SendBaseStatus(c_id, room->GetRoomID());
	}
	room->SetRoundTime(ROUND_TIME);
	//몇 초후에 npc를 어디에 놓을지 정하고 이벤트로 넘기고 초기화 -> 회의 필요
	//g_timer_queue.push( SetTimerEvent(room->GetRoomID(), 
	//	room->GetRoomID(), EVENT_TYPE::EVENT_NPC_SPAWN, 30000));//30초다되면 넣어주는걸로 수정?
	g_timer_queue.push(SetTimerEvent(room->GetRoomID(), room->GetRoomID(),
		EVENT_TYPE::EVENT_TIME, 1000));
	
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
			if (!g_timer_queue.try_pop(ev))continue;
			
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
				g_timer_queue.push(ev);
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
	case EVENT_TYPE::EVENT_NPC_TIMER_SPAWN:
	{
		ex_over->_comp_op = COMP_OP::OP_NPC_TIMER_SPAWN;
		ex_over->target_id = ev.target_id;
		ex_over->room_id = ev.room_id;
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
	case EVENT_TYPE::EVENT_NPC_ATTACK: {
		ex_over->_comp_op = COMP_OP::OP_NPC_ATTACK;
		ex_over->room_id = ev.room_id;
		ex_over->target_id = ev.target_id;
	
		PostQueuedCompletionStatus(hiocp, 1, ev.obj_id, &ex_over->_wsa_over);
		break;
	}
	case EVENT_TYPE::EVENT_TIME: {
		//방으로 처리하도록 바꾸기
		ex_over->_comp_op = COMP_OP::OP_COUNT_TIME;
		ex_over->room_id = ev.obj_id;
		
		PostQueuedCompletionStatus(hiocp, 1, ev.obj_id, &ex_over->_wsa_over);
		break;
	}
	}

	
}
void PacketManager::CreateDBThread()
{

	db_thread=std::thread([this]() {DBThread(); });
}
