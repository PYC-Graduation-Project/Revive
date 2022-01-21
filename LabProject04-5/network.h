#pragma once
#pragma comment (lib, "WS2_32.LIB")
#pragma comment (lib, "MSWSock.LIB")
#include"define.h"
#include <thread>
#include<iostream>

class PacketManager;
class Network
{
public:
	Network() {
		//���߿��� object �����̳ʿ� �ִ� ������Ʈ ��Ŷ���� �ʱ�ȭ
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
	void DestroyWorker()
	{
		worker.join();
	}
private:
	void Worker();
	void DoRecv();
	void OnRecv(int client_id, EXP_OVER* exp_over, DWORD num_byte);
	void ProcessPacket(int c_id,unsigned char*p);
	int GetID() const
	{
		return m_id;
	}
	void CreateWorker()
	{

		worker = thread([this]() {Worker(); });

	}
	
	SOCKET m_s_socket;
	HANDLE m_hiocp;
	EXP_OVER recv_over;
	thread worker;
	int m_id;
	int m_prev_size = 0;
	std::unique_ptr< PacketManager>m_packet_manager;
public:
	//�ӽ� ���� ���Ŀ� Ŭ�󿡼��� �ʿ����
	XMFLOAT3 pos;
	XMFLOAT3 look;
	XMFLOAT3 right;
	//�����̳�-> ��ġ, �浹 ,
};

