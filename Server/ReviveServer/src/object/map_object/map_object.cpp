#include "pch.h"
#include "map_object.h"

void MapObj::SetMinPos()
{
	m_min_pos = Vector3{ m_pos - m_extent };
	
}

void MapObj::SetMaxPos()
{
	m_max_pos = Vector3{ m_pos + m_extent };
}
