#include"stdafx.h"
#include "ingame_server.h"
#include"packet_manager.h"

InGameServer::InGameServer()
{
	m_PacketManager = std::make_unique<PacketManager>();
	m_PacketManager->Init();
}

InGameServer::~InGameServer()
{
}



bool InGameServer::OnRecv(int c_id, EXP_OVER* exp_over, DWORD num_bytes)
{
	m_PacketManager->ProcessRecv(c_id, exp_over, num_bytes);
	return true;
}

void InGameServer::Disconnect(int c_id)
{

}




void InGameServer::Run()
{
	StartServer();
}

void InGameServer::End()
{
	m_PacketManager->End();
	CloseHandle(m_hiocp);
	closesocket(m_s_socket);

}
