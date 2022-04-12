#pragma once
#include<array>
#include"define.h"
class Room;
class RoomManager
{
public:
	RoomManager()=default;
	~RoomManager()
	{
		DestroyRoom();
	}

	void InitRoom();
	void DestroyRoom();
	int GetEmptyRoom();
	Room* GetRoom(int r_id) { return m_rooms[r_id]; }

private:
	std::array<Room*, MAX_ROOM_SIZE>m_rooms;

};

