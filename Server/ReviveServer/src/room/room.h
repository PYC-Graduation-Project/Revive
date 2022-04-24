#pragma once
#include<vector>
#include<mutex>
#include<atomic>
#include<chrono>
#include<array>
#include"object/bullet.h"
class Object;
const int MAX_BULLET=100;
class Room
{
public:
	Room(int room_id) ;
	Room() = default;
	~Room();



	void Init(int user_num);
	void EnterRoom(int c_id);
	

	int GetRoomID() { return room_id; }
	int GetMaxUser() { return max_user; }
	int GetMaxEnemy() { return max_npc; }
	int GetRound() { return curr_round; }
	float GetBaseHp() { return m_base_hp; }
	std::chrono::system_clock::time_point GetRoundTime() { return m_round_time; }
	bool GetState() { return is_ingame; }


	void SetBaseHp(float val) { m_base_hp=val; }
	void SetState(bool val) { is_ingame = val; }
	void SetRoundTime(int seconds);
	void SetRound(int val) { curr_round = val; }
	//Bullet* GetBullet(int id) { return m_bullet_pool[id]; }
	//int GetNewBullet();
	std::vector<int>& GetObjList()
	{
		return m_obj_list;
	}
	std::mutex m_bullet_lock;
private:
	int room_id;
	int max_user;
	std::atomic_bool is_ingame = false;
	int max_npc;
	int curr_round;
	std::vector<int>m_obj_list;
	//std::array<Bullet*, MAX_BULLET>m_bullet_pool;
	std::chrono::system_clock::time_point	m_round_time;
	float m_base_hp = 100.0f;
};


