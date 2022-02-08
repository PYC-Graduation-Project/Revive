#include "pch.h"
#include "enemy.h"
using namespace std;
void Enemy::InitLua(const char* script_name)
{
	L = luaL_newstate();
	luaL_openlibs(L);
	int error = luaL_loadfile(L, script_name) || 
		lua_pcall(L, 0, 0, 0);
	if (error) { cout << "Error : " << lua_tostring(L, -1); lua_pop(L, 1); }

	lua_getglobal(L, "initializEnemy");
	lua_pushnumber(L,m_id);
	lua_pushnumber(L,m_pos.x);
	lua_pushnumber(L,m_pos.y);
	lua_pushnumber(L,m_pos.z);
	lua_pushnumber(L,m_maxhp);
	lua_pushnumber(L,m_damage);
	lua_pushnumber(L,(int)m_type);
	error = lua_pcall(L, 7, 0, 0);
	if (error) { cout << "Error : " << lua_tostring(L, -1); lua_pop(L, 1); }

	RegisterAPI();

}

void Enemy::InitEnemy(OBJ_TYPE type, int room_id, float max_hp, Vector3& pos, float damage)
{
	m_type = type;
	m_room_id = room_id;
	m_hp = max_hp;
	m_maxhp = max_hp;
	SetPos(pos);
	m_damage = damage;
}

void Enemy::RegisterAPI()
{
}
