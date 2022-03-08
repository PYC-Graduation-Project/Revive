#pragma once

#include"server/define.h"
#include <thread>
#include<iostream>
#include"client/math/quaternion.h"
#include"client/math/Vec3.h"
class PacketManager;
class Network
{
private:
	static Network* m_pInst;

public:
	static Network* GetInst()
	{
		if (!m_pInst)
			m_pInst = new Network;
		return m_pInst;
	}

	static void DestroyInst()
	{
		if (m_pInst)
		{
			delete m_pInst;
			m_pInst = NULL;
		}
	}
public:
	Network() {
		//나중에는 object 컨테이너에 있는 오브젝트 패킷으로 초기화
		
	
	
	};
	~Network() {};

	bool Init();
	bool Connect();
	//void error_display(int err_no)
	//{
	//	WCHAR* lpMsgBuf;
	//	FormatMessage(
	//		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
	//		NULL, err_no,
	//		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
	//		(LPTSTR)&lpMsgBuf, 0, 0);
	//	std::wcout << lpMsgBuf << std::endl;
	//	//while (true);
	//	LocalFree(lpMsgBuf);
	//}
	void DestroyWorker()
	{
		worker.join();
	}
	void CreateWorker()
	{

		worker = std::thread([this]() {Worker(); });

	}

	//void SendPacket(int num_byte ,void* packet);

	void SendSignInPacket(char*,char*);
	void SendSignUPPacket(char*,char*);
	void SendMatchingPacket(int user_num);
	void SendMovePacket(client_fw::Vec3& pos, client_fw::Quaternion& rot);

private:
	void Worker();
	void DoRecv();
	void OnRecv(int client_id, EXP_OVER* exp_over, DWORD num_byte);
	//void ProcessPacket(int c_id,unsigned char*p);
	int GetID() const
	{
		return m_id;
	}
	
	
	HANDLE m_hiocp;
	EXP_OVER recv_over;
	std::thread worker;
	std::unique_ptr<PacketManager>m_packet_manager;
	int m_id;
	int m_prev_size = 0;
	//std::unique_ptr< PacketManager>m_packet_manager;
public:
	//임시 변수 이후에 클라에서는 필요없음
	
	//컨테이너-> 위치, 충돌 ,
};

