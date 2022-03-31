#pragma once
#include"object.h"
class MapObj : public Object 
{
public:
	MapObj(int id, const Vector3& pos, const Vector3& extent, const Vector3& scale,bool is_blocking)
		:m_extent(extent),m_is_blocking(is_blocking),m_scale(scale)
	{
		m_id = id;
		m_pos = pos;
		SetMinPos();
		SetMaxPos();
		std::cout << m_min_pos << std::endl;
		std::cout << m_max_pos << std::endl;
	};
	~MapObj()=default;
private:
	
	void SetMinPos();
	void SetMaxPos();
	const Vector3& GetMinPos()const { return m_min_pos; }
	const Vector3& GetMaxPos()const { return m_max_pos; }
	bool m_is_blocking=false;
	Vector3 m_min_pos;
	Vector3 m_max_pos;
	Vector3 m_extent;
	Vector3 m_scale;

};

