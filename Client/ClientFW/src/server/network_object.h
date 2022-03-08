#pragma once
#include"server/define.h"
#include"client/math/vec3.h"
#include<string>
enum class NW_OBJ_TYPE
{
	OT_NONE,
	OT_BASE,
	OT_PLAYER,
	OT_NPC_SKULL,
	OT_NPC_SKULLKING
};
class NetworkObj {
public:
	NetworkObj()=default;
	NetworkObj(int id, float hp, char* name, float x, float y, float z, NW_OBJ_TYPE type)
		:m_id(id), m_hp(hp), m_max_hp(hp),m_type(type)
	{
			m_name = std::string(name);
			m_position =client_fw::Vec3(x, y, z);
	}
	virtual ~NetworkObj()=default;


	const client_fw::Vec3& GetPosition()const {return m_position;}
	const std::string& GetName()const { return m_name; }
	const float GetHp()const { return m_hp; }
	const float GetMaxHp()const { return m_max_hp; }
	const NW_OBJ_TYPE GetType()const { return m_type; }
	const int GetID()const { return m_id; }

	void SetID(int val) { m_id = val; }
	void SetPosition(const client_fw::Vec3& val) { m_position = val; }
	void SetType(const NW_OBJ_TYPE val) { m_type = val; }
	void SetHP(float val) { m_hp = val; }
	void SetMaxHP(float val) { m_max_hp = val; }
	void SetName(const std::string& val) { m_name = val; }
protected:
	float m_hp, m_max_hp;
	std::string m_name;
	client_fw::Vec3 m_position;
	NW_OBJ_TYPE m_type;
	int m_id;

};

