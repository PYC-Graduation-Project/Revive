#include "pch.h"
#include "room.h"


Room::Room(int room_id):room_id(room_id)
{

}

Room::~Room()
{
}

void Room::Init(int user_num, char* name)
{
	//이후에 방비우기로 사용
	ZeroMemory(m_name, sizeof(m_name));
	max_user = user_num;
	max_npc = max_user * NPC_PER_USER;
	strcpy_s(m_name, name);
	
}

void Room::EnterRoom(int c_id)
{
	m_obj_list.insert(c_id);

}



