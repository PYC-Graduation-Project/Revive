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

void InGameServer::OnEvent(EXP_OVER* exp_over)
{
	switch (exp_over->_comp_op)
	{
	case COMP_OP::OP_PLAYER_MOVE: {
		m_PacketManager->UpdateObjMove();
		delete exp_over;
		break;
	}
	}
}

void InGameServer::Disconnect(int c_id)
{
	m_PacketManager->Disconnect(c_id);
}

void InGameServer::DoTimer()
{
	while (true) {
		while (true) {
			timer_event ev;
			if (!m_timer_queue.try_pop(ev))continue;
			auto start_t = chrono::system_clock::now();
			if (ev.start_time <= start_t) {
				ProcessEvent(ev);
			}
			else {
				m_timer_queue.push(ev);
				break;
			}
		}

		this_thread::sleep_for(10ms);
	}
}

void InGameServer::DBThread()
{
	while (true)
	{
		db_task dt;
		if (!m_db_queue.try_pop(dt))
		{
			this_thread::sleep_for(10ms);
			continue;
		}
		ProcessDBTask(dt);
	}
}

void InGameServer::ProcessDBTask(db_task& dt)
{
}

void InGameServer::CreateTimer()
{
	m_worker_threads.emplace_back([this]() {DoTimer(); });
	
}

void InGameServer::CreateDBThread()
{
}

void InGameServer::ProcessEvent(timer_event& ev)
{
	EXP_OVER* ex_over = new EXP_OVER;
	switch (ev.ev) {
	case EVENT_TYPE::EVENT_PLAYER_MOVE: {
		ex_over->_comp_op = COMP_OP::OP_NPC_MOVE;
		break;
	}
	}

	PostQueuedCompletionStatus(m_hiocp, 1, ev.obj_id, &ex_over->_wsa_over);
}




void InGameServer::Run()
{
	

	StartServer();
	CreateTimer();
	JoinThread();
}

void InGameServer::End()
{
	m_PacketManager->End();
	
	CloseHandle(m_hiocp);
	closesocket(m_s_socket);

}
