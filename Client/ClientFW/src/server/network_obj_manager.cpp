#include "stdafx.h"
#include "network_obj_manager.h"



void NetworkObjManager::AddObj(int id, NetworkObj*obj)
{
	if(m_obj_map[id]!=nullptr)
		m_obj_map[id]=obj;
}
