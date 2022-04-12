#pragma once
#include"server/define.h"
<<<<<<< Updated upstream:Client/ClientFW/src/server/packet_manager.h
#include<functional>
#include<unordered_map>

=======
>>>>>>> Stashed changes:Client/Revive/src/packet_manager.h
class PacketManager
{

public:

	PacketManager();
	virtual ~PacketManager();
	
	virtual void Init() {};//처음에 여기서 함수 등록
	void ProcessRecv(int client_id, EXP_OVER* exp_over, DWORD num_byte);
	void ProcessPacket(int c_id, unsigned char* p);
	void RegisterRecvFunction(char key, const std::function<void(int, unsigned char*)>& recv_func);
	

	int GetID() { return m_id; }
	void SetID(int val) { m_id = val; }
	int GetPrevSize() { return m_prev_size; }
	void SetPrevSize(int val) { m_prev_size = val; }

	
protected:
	
	int m_id;
	int m_prev_size;
	
	std::unordered_map<char, std::function<void(int,unsigned char*)> >m_recv_func_map;
};

