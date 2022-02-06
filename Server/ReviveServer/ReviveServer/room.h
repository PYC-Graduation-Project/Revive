#pragma once
#include<unordered_set>
#include<mutex>
#include<atomic>
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

	bool GetState() { return is_ingame; }
	void SetState(bool val) { is_ingame = val; }
	
private:
	int room_id;
	int max_user;
	std::atomic_bool is_ingame = false;
	int max_npc;
	std::mutex m_obj_lock;
	std::unordered_set<int>m_obj_list;

};


