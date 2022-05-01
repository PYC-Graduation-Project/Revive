#include"pch.h"
#include"lua_functions.h"
#include"object/moveobj_manager.h"
#include"game_server/packet_manager.h"
using namespace std;
int API_get_x(lua_State* L)
{
	int user_id =
		(int)lua_tointeger(L, -1);
	lua_pop(L, 2);
	float x= MoveObjManager::GetInst()->GetMoveObj(user_id)->GetPosX();
	lua_pushnumber(L, x);
	return 1;
}

int API_get_y(lua_State* L)
{
	int user_id =
		(int)lua_tointeger(L, -1);
	lua_pop(L, 2);
	float y = MoveObjManager::GetInst()->GetMoveObj(user_id)->GetPosY();
	lua_pushnumber(L, y);
	return 1;
}

int API_get_z(lua_State* L)
{
	int user_id =
		(int)lua_tointeger(L, -1);
	lua_pop(L, 2);
	float z = MoveObjManager::GetInst()->GetMoveObj(user_id)->GetPosZ();
	lua_pushnumber(L, z);
	return 1;
}

int API_test_lua(lua_State* L)
{
	float now_x = (float)lua_tonumber(L, -2);
	float now_y = (float)lua_tonumber(L, -1);
	lua_pop(L, 3);
	
	cout << "test_x: " << now_x << "test_y: " << now_y << endl;
	return 0;
}

int API_attack(lua_State* L)
{
	int target_id = (int)lua_tointeger(L, -1);
	int npc_id = (int)lua_tointeger(L, -2);
	lua_pop(L, 3);
	Enemy* en = MoveObjManager::GetInst()->GetEnemy(npc_id);
	en->SetTargetId(target_id);
	auto attack_t = chrono::system_clock::now();
	timer_event t;
	chrono::milliseconds mil = chrono::duration_cast<chrono::milliseconds>(en->GetAttackTime() - attack_t);
	if (attack_t <=en->GetAttackTime())
		t = PacketManager::SetTimerEvent(npc_id, target_id, en->GetRoomID(), EVENT_TYPE::EVENT_NPC_ATTACK, mil.count());
	else
		t = PacketManager::SetTimerEvent(npc_id, target_id, en->GetRoomID(), EVENT_TYPE::EVENT_NPC_ATTACK, 1000);
		PacketManager::g_timer_queue.push(move(t));
	
	return 0;
}

int API_move(lua_State* L)
{
	int target_id = (int)lua_tointeger(L, -1);
	int npc_id = (int)lua_tointeger(L, -2);
	//cout << "·ç¾Æ npc_id" << target_id << endl;
	lua_pop(L, 3);
	Enemy* en = MoveObjManager::GetInst()->GetEnemy(npc_id);
	en->SetTargetId(target_id);
	timer_event t=PacketManager::SetTimerEvent(en->GetID(), en->GetID(), en->GetRoomID(),EVENT_TYPE::EVENT_NPC_MOVE,50);
	PacketManager::g_timer_queue.push(move(t));
	return 0;
}
