#include "pch.h"
#include "room.h"
#include"object/object.h"
using namespace std;
Room::Room(int room_id):room_id(room_id),max_user(0),max_npc(0)
{
	m_round_time =chrono::system_clock::now();
	m_base = new Object;
}

Room::~Room()
{
}

void Room::Init(int user_num)
{

	//이후에 방비우기로 사용
	max_user = user_num;
	max_npc = user_num * NPC_PER_USER;
	curr_round = 0;
	m_obj_list.reserve(max_user+static_cast<__int64>(max_npc));
}

void Room::EnterRoom(int c_id)
{
	m_obj_list.push_back(c_id);

}

void Room::SetRoundTime(int seconds)
{
	m_round_time = chrono::system_clock::now() + (1ms * seconds);
}



