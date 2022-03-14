#include "stdafx.h"
#include "network_obj_manager.h"

NetworkObjManager* NetworkObjManager::m_pInst = NULL;


void NetworkObjManager::AddObj(int id, NetworkObj*obj)
{
	m_obj_map.try_emplace(id, obj);
}
