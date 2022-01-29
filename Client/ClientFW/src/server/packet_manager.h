#pragma once
#include"server/define.h"
class PacketManager
{
public:
	PacketManager();
	~PacketManager();
	
	void ProcessPacket(int c_id, unsigned char* p);
	void ProcessMove(int c_id, unsigned char* p);
	void ProcessLogin(int c_id, unsigned char* p);
	
};

