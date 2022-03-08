#pragma once
#include<array>
#include"player.h"
#include<unordered_set>
#include"enemy.h"
class MoveObjManager
{
public:
	MoveObjManager() = default;
	~MoveObjManager() = default;

	Player* GetPlayer(int id) { 
		
		Player*p=((Player*)(m_moveobj_arr[id])); 
		return p;
		
	}
	Enemy* GetEnemy(int id) {
		//Enemy* e = ;
		return ((Enemy*)(m_moveobj_arr[id]));
	}
	MoveObj* GetMoveObj(int id) { return m_moveobj_arr[id]; }
	bool IsPlayer(int id) { return (id >= 0) && (id < MAX_USER); }
	bool IsNear(int a, int b);
	bool IsNPC(int id);

	void InitLua(const char* script_name,int obj_id);
	void RegisterAPI(lua_State* L);
	 static int API_get_x(lua_State* L);
	 int API_get_y(lua_State* L);
	 int API_get_z(lua_State* L);

	int GetNewID();
	void Disconnect(int);
	void InitPlayer();
	void InitNPC();
	void DestroyObject();
private:
	std::array <MoveObj*, MAX_USER + MAX_NPC>m_moveobj_arr;
	int m_id;
};

