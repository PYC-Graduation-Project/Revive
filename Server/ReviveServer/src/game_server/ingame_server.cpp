#include "pch.h"
#include "ingame_server.h"
#include"packet_manager.h"
using namespace std;
InGameServer::InGameServer()
{
	m_PacketManager = std::make_unique<PacketManager>();
	m_PacketManager->Init();
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

void InGameServer::OnEvent(int c_id,EXP_OVER* exp_over)
{
	switch (exp_over->_comp_op)
	{
	case COMP_OP::OP_NPC_SPAWN: {
		m_PacketManager->SpawnEnemy(exp_over->target_id);
		delete exp_over;
		break;
	}
	case COMP_OP::OP_PLAYER_MOVE: {
		m_PacketManager->UpdateObjMove();
		delete exp_over;
		break;
	}
	case COMP_OP::OP_NPC_MOVE: {
		m_PacketManager->DoEnemyMove(exp_over->room_id, c_id);
		delete exp_over;
		break;
	}
	case COMP_OP::OP_COUNT_TIME: {
		m_PacketManager->CountTime(exp_over->room_id);
		delete exp_over;
		break;
	}
	case COMP_OP::OP_NPC_ATTACK: {
		m_PacketManager->DoEnemyAttack(c_id,exp_over->target_id,exp_over->room_id);
		delete exp_over;
		break;
	}
	}
}

void InGameServer::Disconnect(int c_id)
{
	m_PacketManager->Disconnect(c_id);
}

void InGameServer::DoTimer(HANDLE hiocp)
{
	m_PacketManager->ProcessTimer(hiocp);
}



void InGameServer::CreateTimer()
{
	m_worker_threads.emplace_back(std::thread(&InGameServer::DoTimer,this,m_hiocp));
	
}



void InGameServer::Run()
{
	

	StartServer();
	CreateTimer();
	m_PacketManager->CreateDBThread();
	JoinThread();
	m_PacketManager->JoinDBThread();
}

void InGameServer::End()
{
	m_PacketManager->End();
	
	CloseHandle(m_hiocp);
	closesocket(m_s_socket);

}
