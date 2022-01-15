#pragma once
#include"define.h"
class PacketManager
{
public:
	PacketManager();
	~PacketManager();
	
	void ProcessRecv(int, EXP_OVER*, DWORD);
private:
	void ProcessPacket(int c_id, unsigned char* p);
	void ProcessMove(int c_id, unsigned char* p);
};

