#pragma once
#include"server/define.h"
#include"client/math/vec3.h"
#include<string>


class NetworkObj {
public:
	NetworkObj()=default;
	NetworkObj(int id,  char* name, float x, float y, float z)
		:m_id(id)
	{
			m_name = std::string(name);
			m_position =client_fw::Vec3(x, y, z);
			m_rotation = client_fw::Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
	}
	~NetworkObj()=default;


	const client_fw::Vec3& GetPosition()const {return m_position;}
	const std::string& GetName()const { return m_name; }
	
	const int GetID()const { return m_id; }
	const client_fw::Quaternion& GetRotation() const { return m_rotation; }
	void SetRotation(const client_fw::Quaternion& rot) { m_rotation = rot; }
	
	void SetID(int val) { m_id = val; }
	void SetPosition(const client_fw::Vec3& val) { m_position = val; }
	
	void SetName(const std::string& val) { m_name = val; }
	
protected:
	std::string m_name;
	client_fw::Vec3 m_position;
	int m_id;
	client_fw::Quaternion m_rotation;
};

