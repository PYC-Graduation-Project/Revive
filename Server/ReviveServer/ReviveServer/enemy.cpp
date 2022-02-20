#include "pch.h"
#include "enemy.h"
using namespace std;


void Enemy::InitEnemy(OBJ_TYPE type, int room_id, float max_hp, Vector3& pos, float damage)
{
	m_type = type;
	m_room_id = room_id;
	m_hp = max_hp;
	m_maxhp = max_hp;
	SetPos(pos);
	m_damage = damage;
}


