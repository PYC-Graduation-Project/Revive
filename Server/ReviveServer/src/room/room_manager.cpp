#include "pch.h"
#include "room_manager.h"
#include"room.h"
void RoomManager::InitRoom()
{
	for (int i = 0; i < MAX_ROOM_SIZE; ++i)
		m_rooms[i] = new Room(i);
}

void RoomManager::DestroyRoom()
{
	for (auto r : m_rooms)
		if (r)delete r;
}

int RoomManager::GetEmptyRoom()
{
	for (auto r : m_rooms)
	{
		if (false == r->GetState())
		{
			r->SetState(true);
			return r->GetRoomID();
		}
	}
	std::cout << "빈방이 없습니다!" << std::endl;
	return -1;
}
