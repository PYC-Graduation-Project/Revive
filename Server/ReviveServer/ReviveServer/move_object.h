#pragma once
#include"object.h"

//�÷��̾�� npc�� �θ�
class MoveObj :public Object
{
public:
	MoveObj() { 
		
		m_rotation = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	};

	virtual ~MoveObj() = default;

	Vector3 GetOriginX()const { return m_origin_pos; }
	

	float GetDamge()const { return m_damage; }
	float GetHP()const { return m_hp; }
	float GetMaxHP()const { return m_maxhp; }
	

	
	Vector4 GetRotation() { return m_rotation; }
	int GetRoomID()const { return m_room_id; }
	bool GetIsActive()const { return m_is_active; }
	char* GetName() { return m_name; }

	void SetIsActive(bool val) { m_is_active = val; }
	
	void SetRotaion(const Vector4& val) { m_rotation = val; }
	void SetDamge(float val) {  m_damage=val; }
	void SetHP(float val)    {  m_hp=val; }
	void SetMaxHP(float val) {  m_maxhp=val; }
	
	void SetRoomID(int val) { m_room_id = val; }

	void SetOriginPos(const Vector3& val) { m_origin_pos = val; }
protected:
	std::atomic_bool m_is_active = false; //�׾��ִ��� ����ִ���
	int m_room_id;
	float m_damage;
	float m_hp, m_maxhp;                      
	
	char m_name[MAX_NAME_SIZE + 1];

	Vector4 m_rotation;
	Vector3 m_origin_pos;
};