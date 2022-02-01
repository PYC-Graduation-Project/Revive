#include "pch.h"
#include "room.h"

Room::Room()
{
}

Room::Room(int room_id):room_id(room_id)
{
	curr_user_count = 0;
}

Room::~Room()
{
}

void Room::Init(int user_num, char* name)
{
	max_user = user_num;
	strcpy_s(m_name, name);
	
}

bool Room::EnterRoom(int c_id)
{
	if (false == CheckUserCount())return false;
	m_obj_list.insert(c_id);
	curr_user_count += 1;
	return true;
}

void Room::LeaveRoom(int c_id)
{
	m_obj_list.erase(c_id);
	curr_user_count -= 1;

}
