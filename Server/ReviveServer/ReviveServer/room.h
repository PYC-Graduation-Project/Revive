#pragma once
#include<vector>
#include<mutex>
#include<atomic>
class Object;
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
	float GetRoundTime() { return m_round_time; }
	bool GetState() { return is_ingame; }
	void SetState(bool val) { is_ingame = val; }
	void SetRoundTime(float val) { m_round_time = val; }
	void SetRound(int val) { curr_round = val; }
	std::vector<int>& GetObjList()
	{
		return m_obj_list;
	}
private:
	int room_id;
	int max_user;
	std::atomic_bool is_ingame = false;
	int max_npc;
	int curr_round;
	std::mutex m_obj_lock;
	std::vector<int>m_obj_list;
	float m_round_time;
	Object* m_base;//방초기화할때 생성
};


