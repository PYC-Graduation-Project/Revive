#pragma once
#include"../object.h"

//플레이어와 npc의 부모
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
	
	virtual void Reset()
	{
		m_is_active = false;
		ZeroMemory(m_name, MAX_NAME_SIZE + 1);
	}
	
	Vector4 GetRotation() { return m_rotation; }
	int GetRoomID()const { return m_room_id; }
	char* GetName() { return m_name; }

	bool GetIsActive()const { return m_is_active.load(std::memory_order_acquire); }
	void SetIsActive(bool val) { m_is_active.store(val,std::memory_order_release); }
	
	void SetRotaion(const Vector4& val) { m_rotation = val; }
	void SetDamge(float val) {  m_damage=val; }
	void SetHP(float val)    {  m_hp=val; }
	void SetMaxHP(float val) {  m_maxhp=val; }
	
	void SetRoomID(int val) { m_room_id = val; }

	void SetOriginPos(const Vector3& val) { m_origin_pos = val; }
	std::mutex m_hp_lock;
protected:
	std::atomic_bool m_is_active = false; //죽어있는지 살아있는지
	int m_room_id;
	float m_damage;
	float m_hp, m_maxhp;                      
	
	char m_name[MAX_NAME_SIZE + 1];

	Vector4 m_rotation;
	Vector3 m_origin_pos;
};