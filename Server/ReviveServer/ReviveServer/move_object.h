#pragma once
#include"object.h"

//플레이어와 npc의 부모
class MoveObj :public Object
{
public:
	MoveObj() { m_state = STATE::ST_FREE; };
	MoveObj(int id, STATE state, OBJ_TYPE type, float hp, float x, float y, float z)
		:m_state(state),m_hp(hp),m_maxhp(hp)
	{
		Init(id, type, x, y, z);
		m_origin_x = x;
		m_origin_y = y;
		m_origin_z = z;
		m_right = Vector3(1.0f, 0.0f, 0.0f);
		m_look = Vector3(0.0f, 1.0f, 0.0f);
		m_up = Vector3(0.0f, 0.0f, 1.0f);
	}

	virtual ~MoveObj() = default;

	float GetOriginX()const { return m_origin_x; }
	float GetOriginY()const { return m_origin_y; }
	float GetOriginZ()const { return m_origin_z; }

	float GetDamge()const { return m_damage; }
	float GetHP()const { return m_hp; }
	float GetMaxHP()const { return m_maxhp; }
	STATE GetState()const { return m_state; }

	Vector3 GetRightVec()const { return m_right; }
	Vector3 GetLookVec()const { return m_look; }
	Vector3 GetUpVec()const { return m_up; }
	
	void SetRightVec(const Vector3& val) { m_right = val; }
	void SetLookVec(const Vector3& val) { m_look = val; }
	void SetUpVec(const Vector3& val) { m_up = val; }

	void SetDamge(float val) {  m_damage=val; }
	void SetHP(float val)    {  m_hp=val; }
	void SetMaxHP(float val) {  m_maxhp=val; }
	void SetState(STATE val) {  m_state=val; }

private:
	std::atomic_bool m_is_active = true; //죽어있는지 살아있는지
	float m_damage;
	float m_hp, m_maxhp;
	STATE m_state;
	Vector3 m_right, m_look, m_up;
	float m_origin_x, m_origin_y, m_origin_z;
};