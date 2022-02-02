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
private:
	std::array<Room*, MAX_ROOM_SIZE>m_rooms;

};

