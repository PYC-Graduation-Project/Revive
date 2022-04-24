#pragma once
#include"util/collision/collisioner.h"
#include"object/object.h"
#include<atomic>
const float BULLET_SCALE = 1.0f;
const float BULLET_RADIUS = 10.0f;
class Bullet:public Object
{
public:
	Bullet(int room_id, int id)
		:m_room_id(room_id)
	{
		m_type = OBJ_TYPE::OT_BULLET;
		m_id = id;
	}

	~Bullet()=default;
	bool operator ==(const Bullet& rhs) {
		return m_id == rhs.GetID();
	}
	const Vector3& GetForward()const { return m_forward; }
	BoxCollision& GetHitCollision() { return m_hit_collision; }
	void Init(const Vector3& pos, const Vector3& max_pos, const Vector3& forward, float speed = 5000.0f)
	{
		m_forward = forward;
		m_speed = speed;
		m_pos = pos;
		m_hit_collision;
	}
	void SetForward(const Vector3& val) { m_forward = val; }
	void SetSpeed(float val) { m_speed = val; };
	const float GetSpeed()const { return m_speed; };
	bool GetInUse()const { return m_in_use.load(std::memory_order_acquire); }
	void SetInUse(bool val) { m_in_use.store(val, std::memory_order_release); }
private:

	Vector3 m_forward;
	BoxCollision m_hit_collision;
	atomic_bool m_in_use = false;
	float m_speed;
	int m_room_id;
};

