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

	virtual void Disconnect(int c_id)=0;
	virtual bool OnRecv(int c_id, EXP_OVER* exp_over, DWORD num_bytes) { return false; };
	virtual bool OnAccept( EXP_OVER* exp_over) { return false; };
	static void error_display(int err_no);
	virtual bool StartServer();
	void DestroyThread();
private:
	//this�� ��Ŀ�� ���ٷ� join ��Ե��� �𸥴� ���߿� �������� �����Լ��� ����
	void CreateWorker();
	void Worker();//�̰Ŵ� ���ΰ� �����Լ��� override 
	
protected:
	SOCKET m_s_socket;
	HANDLE m_hiocp;
	int m_worker_num;
	std::vector <std::thread> m_worker_threads;
};

