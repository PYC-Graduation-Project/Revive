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
