#pragma once
#include"server/define.h"
#include<unordered_map>
#include<iterator>
#include<client/math/quaternion.h>
#include"client/math/vec3.h"
class NetworkObj;
enum class NW_OBJ_TYPE;
class NetworkObjManager
{
private:
	static NetworkObjManager* m_pInst;

public:
	static NetworkObjManager* GetInst()
	{
		if (!m_pInst)
			m_pInst = new NetworkObjManager;
		return m_pInst;
	}

	static void DestroyInst()
	{
		if (m_pInst)
		{
			delete m_pInst;
			m_pInst = NULL;
		}
	}
public:
	NetworkObjManager() {};
	~NetworkObjManager() {};
	void InitObj(int obj_id, NW_OBJ_TYPE m_type, float hp, float damage,client_fw::Vec3& pos);
	void AddObj(int id, NetworkObj*);
	NetworkObj* GetObj(int id) { 
		auto p = m_obj_map.find(id);
		if (p == m_obj_map.end())return nullptr;
		return p->second; 
		
	};
	std::unordered_map<int, NetworkObj*>::const_iterator GetCBegin()
	{
		return m_obj_map.cbegin();
	}
	std::unordered_map<int, NetworkObj*>::const_iterator GetCEnd()
	{
		return m_obj_map.cend();
	}
private:
	std::unordered_map<int,NetworkObj*>m_obj_map;

};

