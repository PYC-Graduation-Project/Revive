#pragma once
#include"object.h"

//�÷��̾�� npc�� �θ�
class MoveObj :public Object
{
public:
	MoveObj()=default;
	MoveObj(int id, STATE state, OBJ_TYPE type, float hp, float x, float y, float z)
		:m_state(state),m_hp(hp),m_maxhp(hp)
	{
		Init(id, type, x, y, z);
		m_origin_x = x;
		m_origin_y = y;
		m_origin_z = z;
	}

	virtual ~MoveObj() = default;

	float GetOriginX()const { return m_origin_x; }
	float GetOriginY()const { return m_origin_y; }
	float GetOriginZ()const { return m_origin_z; }

	float GetDamge()const { return m_damage; }
	float GetHP()const { return m_hp; }
	float GetMaxHP()const { return m_maxhp; }
	STATE GetState()const { return m_state; }

	void SetDamge(float val) {  m_damage=val; }
	void SetHP(float val)    {  m_hp=val; }
	void SetMaxHP(float val) {  m_maxhp=val; }
	void SetState(STATE val) {  m_state=val; }

private:
	std::atomic_bool m_is_active = true; //�׾��ִ��� ����ִ���
	float m_damage;
	float m_hp, m_maxhp;
	STATE m_state;
	float m_origin_x, m_origin_y, m_origin_z;
};