#pragma once
#include"object.h"
class MapObj : public Object 
{
public:
	MapObj(int id,Vector3 pos,Vector3 extent ,bool is_blocking) 
		:m_extent(extent),m_is_blocking(is_blocking)
	{
		m_id = id;
		m_pos = pos;
	};
	~MapObj()=default;

private:
	bool m_is_blocking=false;
	Vector3 m_min_pos;
	Vector3 m_max_pos;
	Vector3 m_extent;

};

