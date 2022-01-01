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

	//this�� ��Ŀ�� ���ٷ� join ��Ե��� �𸥴� ���߿� �������� �����Լ��� ����
	void CreateWorker();
	
	virtual void Worker() = 0;//�̰Ŵ� ���ΰ� �����Լ��� override �Ǵ� �̰Ÿ� override?
	void error_display(int err_no);
protected:
	SOCKET m_s_socket;
	HANDLE m_hiocp;
	int m_worker_num;
	std::vector <std::thread> m_worker_threads;
};

