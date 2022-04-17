#include "pch.h"
#include "enemy.h"
using namespace std;

//기지 아이디= -1
void Enemy::InitEnemy(OBJ_TYPE type, int room_id, 
	float max_hp, Vector3& pos, float damage, const char*name)
{
	m_type = type;
	m_room_id = room_id;
	m_hp = max_hp;
	m_maxhp = max_hp;
	m_L = luaL_newstate();
	SetPos(pos);
	m_damage = damage;
	strcpy_s(m_name, name);
	
}

void Enemy::SetSpawnPoint(float x, float z)
{
	Vector3 pos(x, 300.0f, z);
	SetOriginPos(pos);
	SetPos(pos);
	
}

void Enemy::SetMoveTime(int val)
{
	m_move_time = std::chrono::system_clock::now() + (1ms*val);
}


