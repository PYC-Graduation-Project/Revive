#pragma once
#include"define.h"
#include<concurrent_queue.h>
#include<thread>
//#include<concurrent_priority_queue.h>
class MoveObjManager;
class DB;
class RoomManager;
class MapManager;
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
	void DoEnemyMove(int room_id, int enemy_id);


	void SendMovePacket(int c_id, int mover);
	void SendLoginFailPacket(int c_id, int reason);
	void SendLoginFailPacket(SOCKET& c_socket, int reason);
	void SendSignInOK(int c_id);
	void SendSignUpOK(int c_id);
	void SendMatchingOK(int c_id);
	void SendPutObjPacket(int c_id, int obj_id, OBJ_TYPE obj_type);
	void SendObjInfo(int c_id, int obj_id);
	void SendTime(int c_id,float round_time);
	void SendTestPacket(int c_id, int obj_id, float x, float y, float z);

	timer_event SetTimerEvent(int obj_id, int target_id, EVENT_TYPE ev, int seconds);
	timer_event SetTimerEvent(int obj_id, int target_id,int room_id, EVENT_TYPE ev, int seconds);
	void End();
	void Disconnect(int c_id);

	void CreateDBThread();
	void DBThread();
	void ProcessDBTask(db_task& dt);
	void JoinDBThread();

	void ProcessTimer(HANDLE hiocp);
	void ProcessEvent(HANDLE hiocp, timer_event& ev);

	static concurrency::concurrent_priority_queue <timer_event> g_timer_queue;
private:
	
	
	RoomManager* m_room_manager;
	DB* m_db;
	DB* m_db2;
	MapManager* m_map_manager;
	concurrency::concurrent_queue<db_task>m_db_queue;
	
	std::thread db_thread;


	Vector3 PLAYER_SPAWN_POINT[3]{ //임시 하드코딩
	{2350.0f,300.0f,3150.0f},
	{2450.0f,300.0f,3150.0f},
	{2400.0f,300.0f,3150.0f}
	};
	const float ENEMY_SPAWN_POINT[2][2]
	{
		{1800.f,10200.f},
		{2700.f,10200.f}
	};

	Vector3 BASE_POINT{ 2400.0f,300.0f,2850.0f };
	//Spawn 1
	//min : x=1800,z=10500 max: x=2100,z=10200
	//Spawn 2
	// min x=2700,z=10500, max=3000,z=10200
	//나중에 개발
	void ProcessSignIn (int c_id ,unsigned char *p);
	void ProcessSignUp(int c_id, unsigned char* p);
	void ProcessAttack(int c_id ,unsigned char* p);
	void ProcessMove  (int c_id ,unsigned char* p);
	void ProcessMatching(int c_id, unsigned char* p);
	

	void StartGame(int room_id);
};

