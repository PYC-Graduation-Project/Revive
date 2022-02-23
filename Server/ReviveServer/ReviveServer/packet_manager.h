#pragma once
#include"define.h"
#include<concurrent_queue.h>
#include<thread>

class MoveObjManager;
class DB;
class RoomManager;
class PacketManager
{
public:
	PacketManager();
	~PacketManager() = default;
	
	void Init();
	void ProcessPacket(int c_id, unsigned char* p);
	void ProcessAccept(HANDLE ,SOCKET&,EXP_OVER*);
	void ProcessRecv(int, EXP_OVER*, DWORD);
	
	void UpdateObjMove();
	void SpawnEnemy(int room_id);

	void SendMovePacket(int c_id, int mover);
	void SendLoginFailPacket(int c_id, int reason);
	void SendSignInOK(int c_id);
	void SendSignUpOK(int c_id);
	void SendMatchingOK(int c_id);
	void SendPutObjPacket(int c_id, int obj_id, OBJ_TYPE obj_type);
	void SendObjInfo(int c_id, int obj_id);



	timer_event SetTimerEvent(int obj_id, int target_id, EVENT_TYPE ev, int seconds);
	void End();
	void Disconnect(int c_id);

	void CreateDBThread();
	void DBThread();
	void ProcessDBTask(db_task& dt);
	void JoinDBThread();
private:
	MoveObjManager* m_moveobj_manager;
	RoomManager* m_room_manager;
	DB* m_db;
	DB* m_db2;
	concurrency::concurrent_queue<db_task>m_db_queue;
	std::thread db_thread;
	Vector3 PLAYER_SPAWN_POINT[3]{
	{2350.0f,0.0f,3150.0f},
	{2450.0f,0.0f,3150.0f},
	{2400.0f,0.0f,3150.0f}
	};
	//나중에 개발
	void ProcessSignIn (int c_id ,unsigned char *p);
	void ProcessSignUp(int c_id, unsigned char* p);
	void ProcessAttack(int c_id ,unsigned char* p);
	void ProcessMove  (int c_id ,unsigned char* p);
	void ProcessMatching(int c_id, unsigned char* p);

	void StartGame(int room_id);
};

