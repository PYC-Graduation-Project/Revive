#include "pch.h"
#include "room.h"


Room::Room(int room_id):room_id(room_id),max_user(0),max_npc(0)
{
	
}

Room::~Room()
{
}

void Room::Init(int user_num)
{

	//���Ŀ� ������ ���
	max_user = user_num;
	max_npc = user_num * NPC_PER_USER;
	curr_round = 1;
	m_obj_list.reserve(max_user+static_cast<__int64>(max_npc));
}

void Room::EnterRoom(int c_id)
{
	m_obj_list.push_back(c_id);

}



