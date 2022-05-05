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
	m_prev_pos = pos;
}

void Enemy::Reset()
{
	target_id = -1;
	lua_lock.lock();
	lua_close(m_L);
	lua_lock.unlock();
	m_attack_time = std::chrono::system_clock::now();
	m_check_time = std::chrono::system_clock::now();
	m_move_time = std::chrono::system_clock::now();
	in_use=false;
	in_game = false;
	m_is_active = false;
	ZeroMemory(m_name, MAX_NAME_SIZE + 1);
	m_prev_test_pos=Vector3{ 0.0f,0.0f,0.0f };
}

void Enemy::SetMoveTime(int val)
{
	m_attack_time = std::chrono::system_clock::now() + (1ms*val);
}

void Enemy::DoMove(const Vector3& target_pos)
{
	Vector3& nlook = m_look;
	Vector3& curr_pos = m_pos;
	m_prev_pos = m_pos;
	nlook = target_pos;//Vector3{ target_pos - curr_pos };
	Vector3 move_vec = nlook.Normalrize();
	Vector3 npos = curr_pos + (move_vec * MAX_SPEED);
	m_pos = npos;
	m_collision.UpdateCollision(m_pos);
}

void Enemy::DoPrevMove(const Vector3& target_pos)
{
	Vector3& nlook = m_look;
	Vector3& curr_pos = m_prev_pos;
	nlook = Vector3{ target_pos - curr_pos };
	Vector3 move_vec = nlook.Normalrize();
	Vector3 npos = curr_pos + (move_vec * MAX_SPEED);
	m_prev_pos = m_pos;
	m_pos = npos;
	m_collision.UpdateCollision(m_pos);
}


