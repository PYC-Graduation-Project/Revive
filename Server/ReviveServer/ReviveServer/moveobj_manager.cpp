#include "pch.h"
#include "moveobj_manager.h"
using namespace std;
int MoveObjManager::GetNewID()
{
    Player* cl = NULL;
    for (int i = 0; i < MAX_USER; ++i)
    {
		
        cl = GetPlayer(i);
		
       cl->state_lock.lock();
        if (STATE::ST_FREE == cl->GetState())
        {
            cl->SetState(STATE::ST_ACCEPT);
           cl->state_lock.unlock();
            return i;
        }
		cl->state_lock.unlock();
    }
    cout << "남는아이디 없음" << endl;
    return -1;
}

void MoveObjManager::Disconnect(int c_id)
{
	Player* cl = GetPlayer(c_id);
	
	//for (auto& other : my_vl) {
	//	Player* target = (Player*)clients[other];
	//	if (true == is_npc(target->id)) continue;
	//	if (ST_INGAME != target->state)
	//		continue;
	//	target->m_vl.lock();
	//	if (0 != target->m_viewlist.count(c_id)) {
	//		target->m_viewlist.erase(c_id);
	//		target->m_vl.unlock();
	//		target->send_remove_object(c_id);
	//	}
	//	else target->m_vl.unlock();
	//}
	cl->state_lock.lock();
	closesocket(cl->GetSock());
	cl->SetState ( STATE::ST_FREE);
	cl->state_lock.unlock();
}

void MoveObjManager::InitPlayer()
{
	for (int i = 0; i < MAX_USER; ++i)
		m_moveobj_arr[i] = new Player;
}

void MoveObjManager::DestroyObject()
{
	for (int i = 0; i < MAX_USER; ++i)
	{
		if (STATE::ST_INGAME == m_moveobj_arr[i]->GetState())
		{
			Disconnect(m_moveobj_arr[i]->GetID());
			delete m_moveobj_arr[i];
		}
	}

}


