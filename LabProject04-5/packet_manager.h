#pragma once
#include"define.h"
#include<array>
class CPlayer;
class PacketManager
{
public:
	PacketManager() = default;
	~PacketManager() = default;
	void Init();
	void ProcessPacket(CPlayer*, unsigned char* p);

	void ProcessRecv(CPlayer*, EXP_OVER*, DWORD);
	void End();

private:

	void ProcessLogin();

	//���߿� �����ϱ�
	void ProcessAttack();
	void ProcessMove();
};

