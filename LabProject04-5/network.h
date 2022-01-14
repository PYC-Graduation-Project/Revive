#pragma once
#pragma comment (lib, "WS2_32.LIB")
#pragma comment (lib, "MSWSock.LIB")
#include"define.h"
#include <thread>
#include<iostream>

class Network
{
public:
	Network() {
		pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		right = XMFLOAT3(1.0f, 0.0f, 0.0f);
		
		look = XMFLOAT3(0.0f, 0.0f, 1.0f);
	
	
	};
	~Network() {};

	bool Init();
	bool Connect();
	void error_display(int err_no)
	{
		WCHAR* lpMsgBuf;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, err_no,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf, 0, 0);
		std::wcout << lpMsgBuf << std::endl;
		//while (true);
		LocalFree(lpMsgBuf);
	}

	void Worker();
	void DoRecv();
	void ProcessPacket(int c_id,unsigned char*p);

	void CreateWorker()
	{
		
		workers.emplace_back([this]() {Worker(); });
		
	}
	SOCKET m_s_socket;
	HANDLE m_hiocp;
	EXP_OVER recv_over;
	vector<thread>workers;
	int m_id;
	int m_prev_size = 0;
	int m_curr_size = 0;
	XMFLOAT3 pos;
	XMFLOAT3 look;
	XMFLOAT3 right;
};

