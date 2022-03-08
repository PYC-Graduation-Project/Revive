#pragma once
#include"server/define.h"
#include<vector>
#include<iostream>
class PacketManager
{

public:

	PacketManager();
	~PacketManager();
	
	void Init();
	void ProcessRecv(int client_id, EXP_OVER* exp_over, DWORD num_byte);
	void ProcessPacket(int c_id, unsigned char* p);
	
	


	//DirectX::XMFLOAT3 pos;
	//DirectX::XMFLOAT3 look;
	//DirectX::XMFLOAT3 right;
	SOCKET& GetSock() { return m_s_socket; }
	int GetID() { return m_id; }
	void SetID(int val) { m_id = val; }
	int GetPrevSize() { return m_prev_size; }
	void SetPrevSize(int val) { m_prev_size = val; }
	void SendPacket(int num_byte, void* packet);
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
private:
	void ProcessMove(int c_id, unsigned char* p);
	void ProcessSignIn(int c_id, unsigned char* p);
	void ProcessSignUp(int c_id, unsigned char* p);
	void ProcessLoginFali(int c_id, unsigned char* p);
	void ProcessMatching(int c_id, unsigned char* p);
	void ProcessObjInfo(int c_id, unsigned char* p);
	SOCKET m_s_socket;
	int m_id;
	int m_prev_size = 0;
};

