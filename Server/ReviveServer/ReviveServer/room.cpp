#include "pch.h"
#include "room.h"


Room::Room(int room_id):room_id(room_id)
{

}

Room::~Room()
{
}

void Room::Init(int user_num)
{
	//이후에 방비우기로 사용
	max_user = user_num;
	max_npc = max_user * NPC_PER_USER;
	m_obj_list.reserve(max_npc + max_user);
}

void Room::EnterRoom(int c_id)
{
	m_obj_list.push_back(c_id);

}



