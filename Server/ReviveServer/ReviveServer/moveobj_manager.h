#pragma once
#include<array>
#include"player.h"
class MoveObjManager
{
public:
	MoveObjManager() = default;
	~MoveObjManager() = default;

	Player* GetPlayer(int id) { return reinterpret_cast<Player*>(m_moveobj_arr[id]); }
	bool IsPlayer(int id) { return (id >= 0) && (id < MAX_USER); }
	bool IsNear(int a, int b);
	bool IsNPC(int id);


	int GetNewID();
	void Disconnect(int);
	void InitPlayer();

private:
	std::array <MoveObj*, MAX_USER + MAX_NPC>m_moveobj_arr;
	int m_id;
};

