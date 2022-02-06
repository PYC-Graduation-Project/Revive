#include "pch.h"
#include "enemy.h"
using namespace std;
void Enemy::Init(const char* script_name)
{
	L = luaL_newstate();
	luaL_openlibs(L);
	int error = luaL_loadfile(L, script_name) || 
		lua_pcall(L, 0, 0, 0);
	if (error) { cout << "Error : " << lua_tostring(L, -1); lua_pop(L, 1); }

	lua_getglobal(L, "initializEnemy");
	lua_pushnumber(L,m_id);
	lua_pushnumber(L,m_x);
	lua_pushnumber(L,m_y);
	lua_pushnumber(L,m_z);
	lua_pushnumber(L,m_maxhp);
	lua_pushnumber(L,m_damage);
	lua_pushnumber(L,(int)m_type);
	error = lua_pcall(L, 7, 0, 0);
	if (error) { cout << "Error : " << lua_tostring(L, -1); lua_pop(L, 1); }

	RegisterAPI();

}

void Enemy::RegisterAPI()
{
}
