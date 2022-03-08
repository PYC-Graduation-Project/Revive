#include "stdafx.h"
#include "network_obj_manager.h"
NetworkObjManager* NetworkObjManager::m_pInst = NULL;

void NetworkObjManager::InitObj(int obj_id, NW_OBJ_TYPE m_type, float hp, float damage, client_fw::Vec3& pos)
{
}

void NetworkObjManager::AddObj(int id, NetworkObj*obj)
{
	m_obj_map.try_emplace(id, obj);
}
