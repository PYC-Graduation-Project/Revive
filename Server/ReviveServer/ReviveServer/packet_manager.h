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
	void ProcessRecv(int, EXP_OVER*, DWORD);
	void UpdateObjMove();
	void SendMovePacket(int c_id, int mover);
	timer_event SetTimerEvent(int obj_id, int target_id, EVENT_TYPE ev, int seconds);
	void End();
	void Disconnect(int c_id);
private:
	MoveObjManager* m_moveobj_manager;
	
	//나중에 개발
	void ProcessLogin (int c_id ,unsigned char *p);
	void ProcessAttack(int c_id ,unsigned char* p);
	void ProcessMove  (int c_id ,unsigned char* p);
};

