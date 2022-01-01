#pragma once
#pragma comment (lib, "WS2_32.LIB")
#pragma comment (lib, "MSWSock.LIB")

#include"define.h"
#include <thread>
#include <vector>
#include<array>


class IOCPServer
{
public:
	IOCPServer();
	virtual ~IOCPServer();

	bool Init(const int);
	bool BindListen(const int);

	//this의 워커를 람다로 join 어떻게될지 모른다 나중에 오류나면 가상함수로 수정
	void CreateWorker();
	
	virtual void Worker() = 0;//이거는 냅두고 내부함수를 override 또는 이거를 override?
	void error_display(int err_no);
protected:
	SOCKET m_s_socket;
	HANDLE m_hiocp;
	int m_worker_num;
	std::vector <std::thread> m_worker_threads;
};

