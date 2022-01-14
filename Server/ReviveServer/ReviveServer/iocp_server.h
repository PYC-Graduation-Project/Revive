#pragma once
#pragma comment (lib, "WS2_32.LIB")
#pragma comment (lib, "MSWSock.LIB")

#include"define.h"
#include <thread>
#include <vector>
#include<array>
#include<concurrent_priority_queue.h>

class IOCPServer
{
public:
	IOCPServer();
	virtual ~IOCPServer();

	bool Init(const int);
	bool BindListen(const int);

	virtual void Disconnect(int c_id) {}
	virtual bool OnRecv(int c_id, EXP_OVER* exp_over, DWORD num_bytes) { return true; };
	virtual bool OnAccept( EXP_OVER* exp_over) { return false; };
	virtual void OnEvent(EXP_OVER* exp_over) { return; };
	static void error_display(int err_no);
	virtual bool StartServer();
	virtual void ProcessEvent(timer_event& ev);
	void DestroyThread();
public:
	//this의 워커를 람다로 join 어떻게될지 모른다 나중에 오류나면 가상함수로 수정
	void CreateWorker();
	void CreateTimer();
	void Worker();//이거는 냅두고 내부함수를 override 
	void DoTimer();
protected:
	EXP_OVER	accept_ex;
	SOCKET m_s_socket;
	HANDLE m_hiocp;
	int m_worker_num;
	std::vector <std::thread> m_worker_threads;
	concurrency::concurrent_priority_queue <timer_event> m_timer_queue;

};

