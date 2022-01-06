#pragma once
#include"define.h"
class MoveObjManager;

class PacketManager
{
public:
	PacketManager() = default;
	~PacketManager() = default;
	void Init();
	void ProcessPacket(int c_id, unsigned char* p);
	void ProcessAccept(HANDLE ,SOCKET&,EXP_OVER*);
private:
	MoveObjManager* m_moveobj_manager;
	void ProcessLogin();

	//나중에 개발하기
	void ProcessAttack();
	void ProcessMove();
};

