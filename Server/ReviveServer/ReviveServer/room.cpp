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
	//���Ŀ� ������ ���
	max_user = user_num;
	max_npc = max_user * NPC_PER_USER;
	
}

void Room::EnterRoom(int c_id)
{
	m_obj_list.insert(c_id);

}



