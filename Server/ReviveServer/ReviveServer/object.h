#pragma once
#include"pch.h"
class Object
{
public:
	Object(){
		m_type = OBJ_TYPE::OT_MAPOBJ;
		m_pos = Vector3(0.0f, 0.0f, 0.0f);
	};
	
	virtual ~Object() = default;


	void Init(int id, OBJ_TYPE type, float x, float y, float z)
	{
		m_id = id;
		m_type = type;
		m_pos.x = x;
		m_pos.y = y;
		m_pos.z = z;
	};

	float GetPosX()const { return m_pos.x; };
	float GetPosY()const { return m_pos.y; };
	float GetPosZ()const { return m_pos.z; };
	Vector3& GetPos() { return m_pos; }
	

	int GetID()const   { return m_id; };
	OBJ_TYPE GetType()const { return m_type; };


	void SetID(int val)   { m_id = val; };
	void SetType(OBJ_TYPE val) { m_type = val; }

	void SetPosX(float val) { m_pos.x = val; };
	void SetPosY(float val) {m_pos.y = val; };
	void SetPosZ(float val) {m_pos.z = val; };
	void SetPos(const Vector3& val) { m_pos = val; }
protected:
	int m_id;
	OBJ_TYPE m_type;
	Vector3 m_pos;
	


};