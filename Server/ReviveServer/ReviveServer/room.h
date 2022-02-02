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



	void Init(int user_num, char* name);
	bool EnterRoom(int c_id);
	void LeaveRoom(int c_id);


	bool CheckUserCount(){ return max_user > curr_user_count; }

	int GetRoomID() { return room_id; }
	int GetMaxUser() { return max_user; }
	int GetCurrUserCount() { return curr_user_count; }


private:
	int room_id;
	int max_user;
	std::atomic_int curr_user_count;
	int max_npc;
	char m_name[MAX_NAME_SIZE];
	std::mutex m_obj_lock;
	std::unordered_set<int>m_obj_list;

};


