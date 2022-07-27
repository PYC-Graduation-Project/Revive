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
	void SpawnEnemyByTime(int enemy_id,int room_id);
	void DoEnemyMove(int room_id, int enemy_id);
	void CountTime(int room_id);
	void DoEnemyAttack(int enemy_id, int target_id, int room_id);
	void BaseAttackByTime(int room_id,int enemy_id);
	void ActivateHealEvent(int room_id, int player_id);

	void SendMovePacket(int c_id, int mover);
	void SendNPCAttackPacket(int c_id,int obj_id, int target_id);
	void SendLoginFailPacket(int c_id, int reason);
	void SendLoginFailPacket(SOCKET& c_socket, int reason);
	void SendSignInOK(int c_id);
	void SendSignUpOK(int c_id);
	void SendMatchingOK(int c_id);
	void SendPutObjPacket(int c_id, int obj_id, OBJ_TYPE obj_type);
	void SendObjInfo(int c_id, int obj_id);
	void SendTime(int c_id,float round_time);
	void SendTestPacket(int c_id, int obj_id, float x, float y, float z);
	void SendAttackPacket(int c_id, int attacker,const Vector3&forward_vec);
	void SendBaseStatus(int c_id ,int room_id);
	void SendStatusChange(int c_id, int obj_id, float hp);
	void SendGameWin(int c_id);
	void SendGameDefeat(int c_id);
	void SendDead(int c_id,int obj_id);
	void SendWaveInfo(int c_id,int curr_round, int king_num, int sordier_num);
	



	void End();
	void Disconnect(int c_id);
	bool IsRoomInGame(int room_id);
	void EndGame(int room_id);



	void CreateDBThread();
	void DBThread();
	void ProcessDBTask(db_task& dt);
	void JoinDBThread();

	void ProcessTimer(HANDLE hiocp);
	void ProcessEvent(HANDLE hiocp, timer_event& ev);

	static timer_event SetTimerEvent(int obj_id, int target_id, EVENT_TYPE ev, int seconds);
	static timer_event SetTimerEvent(int obj_id, int target_id,int room_id, EVENT_TYPE ev, int seconds);
	static concurrency::concurrent_priority_queue <timer_event> g_timer_queue;
private:
	
	
	RoomManager* m_room_manager;
	//MoveObjManager* m_move_obj_manager;
	DB* m_db;
	DB* m_db2;
	MapManager* m_map_manager;
	concurrency::concurrent_queue<db_task>m_db_queue;
	
	std::thread db_thread;

	bool CheckPlayerBullet(int c_id, const Vector3& position, const Vector3& forward);
	void CallStateMachine(int enemy_id, int room_id,const Vector3& base_pos);
	bool CheckMoveOK(int enemy_id,int room_id);

	//나중에 개발
	void ProcessSignIn (int c_id ,unsigned char *p);
	void ProcessSignUp(int c_id, unsigned char* p);
	void ProcessAttack(int c_id ,unsigned char* p);
	void ProcessMove  (int c_id ,unsigned char* p);
	void ProcessMatching(int c_id, unsigned char* p);
	void ProcessHit(int c_id, unsigned char* p);
	void ProcessGameStart(int c_id, unsigned char* p);
	void ProcessDamageCheat(int c_id, unsigned char* p);
	void StartGame(int room_id);
};

