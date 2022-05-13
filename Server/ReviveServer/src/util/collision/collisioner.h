#pragma once
#include"util/vec3.h"

const float SOLDIER_SCALE = 0.5f;
const float KING_SCALE = 0.6f;
const Vector3 SOLDIER_LOCAL_POS{ 0.0f,100.0f,0.0f };
const Vector3 KING_LOCAL_POS{ 0.0f,200.0f,0.0f };
const Vector3 SOLDIER_EXTENT{ 30.0f,100.0f,40.0f };
const Vector3 KING_EXTENT{ 60.0f,200.0f,60.0f };



class Collisioner {
public:
	Collisioner(const Vector3& player_position, const Vector3& local_position = Vector3{ 0.0f,0.0f,0.0f }, const float scale = 1.0f)
		:m_local_pos(local_position),m_scale(scale)
	{
		m_center_pos = player_position + (local_position*scale);
	};
	Collisioner()=default;
	virtual~Collisioner() {};

	virtual void UpdateCollision(const Vector3& new_position) {};
	
	Vector3& GetLocalPos() { return m_local_pos; }
	Vector3& GetCenterPos() { return m_center_pos; }


	void SetLocalPos(const Vector3& val) { m_local_pos = val; }
	void SetCenterPos(const Vector3& val) { m_center_pos = val; }
protected:
	
	float m_scale;
	Vector3 m_local_pos;
	Vector3 m_center_pos;
};

class BoxCollision
	:public Collisioner
{
public:
	BoxCollision() = default;
	BoxCollision(const Vector3& player_position, const Vector3& local_position, const Vector3& extent,const float scale)
		//:Collisioner(player_position, local_position,scale)
	{
		m_center_pos = player_position + (local_position * scale);
		m_local_pos = local_position;
		m_scale = scale;
		m_extent = extent * scale;
		m_min_pos = m_center_pos - extent;
		m_max_pos = m_center_pos + extent;
	}
	BoxCollision(const Vector3& position, const Vector3& extent)
		:Collisioner(position)
	{
		m_extent = extent ;
		m_center_pos = position;
		m_min_pos = m_center_pos - extent;
		m_max_pos = m_center_pos + extent;
	}
	virtual ~BoxCollision() = default;
public:
	virtual void UpdateCollision(const Vector3& new_position)override 
	{
		m_center_pos = new_position+m_local_pos;
		m_min_pos = m_center_pos - m_extent;
		m_max_pos = m_center_pos + m_extent;
	
	};
	Vector3& GetMinPos() { return m_min_pos; }
	Vector3& GetMaxPos() { return m_max_pos; }
	void SetMinPos(const Vector3& val) { m_min_pos = val; }
	void SetMaxPos(const Vector3& val) { m_max_pos = val; }
private:
	Vector3 m_min_pos;
	Vector3 m_max_pos;
	Vector3 m_extent;
};

class SphereCollison 
	:public Collisioner
{
public:
	SphereCollison() = default;
	SphereCollison(const Vector3& player_position, const Vector3& local_position,const float scale, const float radius)
		:Collisioner(player_position, local_position,scale)
	{
		m_radius = radius * scale;
	}
	virtual ~SphereCollison() = default;
	virtual void UpdateCollision(const Vector3& new_position)override
	{
		m_center_pos = new_position + m_local_pos;

	};
	float GetRadius() { return m_radius; }
	float SetRadius(const float val) { m_radius = val; }
private:
	float m_radius;
};
