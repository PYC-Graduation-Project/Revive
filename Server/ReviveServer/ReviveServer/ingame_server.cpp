#include "pch.h"
#include "ingame_server.h"
#include"packet_manager.h"
using namespace std;
InGameServer::InGameServer()
{
}

InGameServer::~InGameServer()
{
}


bool InGameServer::OnAccept(EXP_OVER* exp_over)
{
	m_PacketManager->ProcessAccept(m_hiocp, m_s_socket, exp_over);
	return true;
}

bool InGameServer::OnRecv(int c_id, EXP_OVER* exp_over, DWORD num_bytes)
{
	m_PacketManager->ProcessRecv(c_id, exp_over, num_bytes);
	return true;
}

void InGameServer::Disconnect(int c_id)
{
	m_PacketManager->Disconnect(c_id);
}




void InGameServer::Run()
{
	m_PacketManager = std::make_unique<PacketManager>();
	m_PacketManager->Init();

	StartServer();
}

void InGameServer::End()
{
	m_PacketManager->End();
	CloseHandle(m_hiocp);
	closesocket(m_s_socket);

}
