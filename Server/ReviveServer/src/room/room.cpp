#include "pch.h"
#include "room.h"
#include"object/object.h"
using namespace std;
Room::Room(int room_id):room_id(room_id),max_user(0),max_npc(0)
{
	m_round_time =chrono::system_clock::now();
	m_base = new Object;
	for (int i = 0; i < MAX_BULLET; ++i)
	{
		m_bullet_pool[i] = new Bullet(room_id,i);
	}
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

int Room::GetNewBullet()
{
	
	for (int i = 0; i < MAX_BULLET; ++i)
	{

		if (false == m_bullet_pool[i]->GetInUse())
		{
			m_bullet_pool[i]->SetInUse(true);
			
			return i;
		}
		
	}
	cout << "남는아이디 없음" << endl;
	return -1;
	
}



