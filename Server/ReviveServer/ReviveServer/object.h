#pragma once
#include"pch.h"
class Object
{
public:
	Object():m_x(0.0f),m_y(0.0f),m_z(0.0f) {};
	virtual ~Object() = default;


	void Init(int id, OBJ_TYPE type, float x, float y, float z)
	{
		m_id = id;
		m_type = type;
		m_x = x;
		m_y = y;
		m_z = z;
	};

	float GetPosX()const { return m_x; };
	float GetPosY()const { return m_y; };
	float GetPosZ()const { return m_z; };

	

	int GetID()const   { return m_id; };
	OBJ_TYPE GetType()const { return m_type; };


	void SetID(int val)   { m_id = val; };
	void SetType(OBJ_TYPE val) { m_type = val; }

	void SetPosX(float val) { m_x = val; };
	void SetPosY(float val) { m_y = val; };
	void SetPosZ(float val) { m_z = val; };
private:
	int m_id;
	OBJ_TYPE m_type;
	float m_x, m_y, m_z;
	


};