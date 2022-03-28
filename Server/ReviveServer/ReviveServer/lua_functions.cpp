#include"pch.h"
#include"lua_functions.h"
#include"moveobj_manager.h"

using namespace std;
int API_get_x(lua_State* L)
{
	int user_id =
		(int)lua_tointeger(L, -1);
	lua_pop(L, 2);
	float x = MoveObjManager::GetInst()->GetPlayer(user_id)->GetPosX();
	lua_pushnumber(L, x);
	return 1;
}

int API_get_y(lua_State* L)
{
	int user_id =
		(int)lua_tointeger(L, -1);
	lua_pop(L, 2);
	float y = MoveObjManager::GetInst()->GetPlayer(user_id)->GetPosY();
	lua_pushnumber(L, y);
	return 1;
}

int API_get_z(lua_State* L)
{
	int user_id =
		(int)lua_tointeger(L, -1);
	lua_pop(L, 2);
	float z = MoveObjManager::GetInst()->GetPlayer(user_id)->GetPosZ();
	lua_pushnumber(L, z);
	return 1;
}

int API_test_lua(lua_State* L)
{
	float now_x = (float)lua_tonumber(L, -2);
	float now_y = (float)lua_tonumber(L, -1);
	lua_pop(L, 3);
	cout << "test_x: " << now_x << "test_y: " << now_y << endl;
	return 1;
}
