#include "pch.h"
#include "moveobj_manager.h"


using namespace std;





bool MoveObjManager::IsNear(int a, int b)
{
	Vector3 obj_a = m_moveobj_arr[a]->GetPos();
	Vector3 obj_b = m_moveobj_arr[b]->GetPos();
	if (FOV_RANGE < sqrt(pow(abs(obj_a.x - obj_b.x), 2) + pow(abs(obj_a.z - obj_b.z), 2)))
		return false;
	return true;
}

void MoveObjManager::InitLua(const char* script_name, int obj_id)
{
	Enemy* en = GetEnemy(obj_id);
	lua_State*L = en->GetLua();
	L = luaL_newstate();
	luaL_openlibs(L);
	
	int error = luaL_loadfile(L, script_name) ||
		lua_pcall(L, 0, 0, 0);
	if (error) { cout << "Error : " << lua_tostring(L, -1); lua_pop(L, 1); }

	lua_getglobal(L, "initializEnemy");
	lua_pushnumber(L, en->GetID());
	lua_pushnumber(L, en->GetPosX());
	lua_pushnumber(L, en->GetPosY());
	lua_pushnumber(L, en->GetPosZ());
	lua_pushnumber(L, en->GetHP());
	lua_pushnumber(L, en->GetDamge());
	lua_pushnumber(L, (int)en->GetType());
	error = lua_pcall(L, 7, 0, 0);
	if (error) { cout << "Error : " << lua_tostring(L, -1); lua_pop(L, 1); }
	
	//RegisterAPI( L);
}

//void MoveObjManager::RegisterAPI(lua_State* L)
//{
//
//	lua_register(L, "API_get_x", API_get_x);
//	lua_register(L, "API_get_y", API_get_y);
//	lua_register(L, "API_get_z", API_get_z);
//}

int MoveObjManager::API_get_x(lua_State* L)
{
	int user_id =
		(int)lua_tointeger(L, -1);
	lua_pop(L, 2);
	float x;
	lua_pushnumber(L, x);
	return 1;
}

int MoveObjManager::API_get_y(lua_State* L)
{
	int user_id =
		(int)lua_tointeger(L, -1);
	lua_pop(L, 2);
	float y = m_moveobj_arr[user_id]->GetPosY();
	lua_pushnumber(L, y);
	return 1;
}

int MoveObjManager::API_get_z(lua_State* L)
{
	int user_id =
		(int)lua_tointeger(L, -1);
	lua_pop(L, 2);
	float z = m_moveobj_arr[user_id]->GetPosZ();
	lua_pushnumber(L, z);
	return 1;
}

int MoveObjManager::GetNewID()
{
    Player* cl = NULL;
    for (int i = 0; i < MAX_USER; ++i)
    {
		
        cl = GetPlayer(i);
		
       cl->state_lock.lock();
        if (STATE::ST_FREE == cl->GetState())
        {
            cl->SetState(STATE::ST_ACCEPT);
           cl->state_lock.unlock();
            return i;
        }
		cl->state_lock.unlock();
    }
    cout << "남는아이디 없음" << endl;
    return -1;
}

void MoveObjManager::Disconnect(int c_id)
{
	Player* cl = GetPlayer(c_id);
	//매칭취소 넣기
	//for (auto& other : my_vl) {
	//	Player* target = (Player*)clients[other];
	//	if (true == is_npc(target->id)) continue;
	//	if (ST_INGAME != target->state)
	//		continue;
	//	target->m_vl.lock();
	//	if (0 != target->m_viewlist.count(c_id)) {
	//		target->m_viewlist.erase(c_id);
	//		target->m_vl.unlock();
	//		target->send_remove_object(c_id);
	//	}
	//	else target->m_vl.unlock();
	//}
	cl->is_matching = false;
	cl->state_lock.lock();
	closesocket(cl->GetSock());
	cl->SetState ( STATE::ST_FREE);
	cl->state_lock.unlock();
}

void MoveObjManager::InitPlayer()
{
	for (int i = 0; i < MAX_USER; ++i)
		m_moveobj_arr[i] = new Player;

	
}

void MoveObjManager::InitNPC()
{
	for (int i = NPC_ID_START; i <= NPC_ID_END; ++i)
		m_moveobj_arr[i] = new Enemy(i);
}

void MoveObjManager::DestroyObject()
{
	//for (int i = 0; i < MAX_USER; ++i)
	//{
	//	if (STATE::ST_INGAME == m_moveobj_arr[i]->GetState())
	//	{
	//		Disconnect(m_moveobj_arr[i]->GetID());
	//		delete m_moveobj_arr[i];
	//	}
	//}
	for (auto obj : m_moveobj_arr)
	{
		if (true == IsPlayer(obj->GetID()))
		{
			if (GetPlayer(obj->GetID())->GetState() != STATE::ST_FREE)
				Disconnect(obj->GetID());
		}
		if (obj)
		{
			delete obj;
			obj = NULL;
		}
	}
}


