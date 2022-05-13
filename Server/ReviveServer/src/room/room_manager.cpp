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
		r->m_state_lock.lock();
		if (ROOM_STATE::RT_FREE == r->GetState())
		{
			r->SetState(ROOM_STATE::RT_INGAME);
			r->m_state_lock.unlock();
			return r->GetRoomID();
		}
		r->m_state_lock.unlock();
	}
	std::cout << "빈방이 없습니다!" << std::endl;
	return -1;
}
