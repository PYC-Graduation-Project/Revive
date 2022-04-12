#include "pch.h"
#include "Astar.h"
#include"object/map_object/map_object.h"
#include<iostream>
#include<cmath>

using namespace std;
bool Astar::SearchAllPath(const vector<MapObj>& map_objects, const Vector3& start_pos, const Vector3& dst_pos)
{
	Vec2 dst=GetDestination(start_pos,dst_pos);
	Vec2 src= m_start_pos;
	if (false == IsInRange(dst))return false;
	if (true == IsBlocked(map_objects, dst))
	{
		cout << "������" << endl;
		return false;
	}

	Node start_node = testing_map[m_start_pos.first][m_start_pos.second];
	start_node.pos = m_start_pos;
	start_node.parant_pos = m_start_pos;
	start_node.f = 0;
	start_node.g = 0;
	start_node.h = GethValue(m_start_pos.first, m_start_pos.second, dst);
	open_pq.push(start_node);

	while (!open_pq.empty())
	{
		Node now = open_pq.top();
		open_pq.pop();

		if (now.pos == dst)
		{
			TracePath(now, src, dst);
			return true;
		}
		if (close_set.find(now.pos) != close_set.end())
			continue;
		int x = now.pos.second;
		int y = now.pos.first;

		close_set.insert(now.pos);
		for (int i = 0; i < 8; ++i)
		{
			int nx = now.pos.second + dirX[i];
			int ny = now.pos.first + dirY[i];
			Vec2 new_pos{ ny,nx };
			if (false == IsInRange(new_pos))continue;
			if (true == IsBlocked(map_objects, new_pos))continue;
			if (close_set.find(new_pos) != close_set.end())continue;
			int ng = now.g + cost[i];
			int nh = GethValue(ny, nx, dst);
			int nf = ng + nh;
			if (testing_map[ny][nx].f < nf)
				continue;
			testing_map[ny][nx].f = nf;
			testing_map[ny][nx].g = ng;
			testing_map[ny][nx].h = nh;
			testing_map[ny][nx].pos = new_pos;
			testing_map[ny][nx].parant_pos = now.pos;
			open_pq.push(testing_map[ny][nx]);

		}

	}

	return false;
}

bool Astar::IsBlocked(const vector<MapObj>& map_objects, const Vec2& dst)
{
	float x = (dst.second  * REAL_DISTANCE )+ m_zero_position.x;
	float z = (dst.first * REAL_DISTANCE) + m_zero_position.z;
	for (auto map_obj : map_objects)
	{
		if (OBJ_TYPE::OT_SPAWN_AREA == map_obj.GetType())continue;
		else if (OBJ_TYPE::OT_ACTIViTY_AREA == map_obj.GetType()) 
		{
			if (map_obj.GetMinPos().x > x && map_obj.GetMaxPos().x < x &&
				map_obj.GetMinPos().z > z && map_obj.GetMaxPos().z < z)
			{

				//test_map[dst.first][dst.second] = '1';
				return true;

			}
			
		}
		else if (OBJ_TYPE::OT_BASE == map_obj.GetType())
		{
			if (abs(map_obj.GetPosX() - x) <= 10.0f && abs(map_obj.GetPosZ() - z) <= 10.0f)
			{
				return true;
			}
		}
		else {
			if (map_obj.GetMinPos().x <= x && map_obj.GetMaxPos().x >= x &&
				map_obj.GetMinPos().z <= z && map_obj.GetMaxPos().z >= z)
			{

				//test_map[dst.first][dst.second] = '1';
				return true;

			}
		}
		
		
	}
	return false;
}

bool Astar::IsInRange( const Vec2& pos)
{

	return(pos.first >= 0 && pos.first < COL&& pos.second >= 0 && pos.second < ROW);
}

void Astar::TracePath(Node& now, Vec2 src, Vec2 dst)
{  
	Node node = now;
	std::vector<Vec2>result_vec;
	std::cout << "����" << std::endl;
	while (testing_map[node.pos.first][node.pos.second].parant_pos != m_start_pos)
	{
		result_vec.push_back(node.parant_pos);
		//test_map[node.parant_pos.first][node.parant_pos.second] = '*';
		node = testing_map[node.parant_pos.first][node.parant_pos.second];
	}
	//for (int i = 0; i < ROW; ++i)
	//{
	//	for (int j = 0; j < COL; ++j)
	//	{
	//		
	//		cout << test_map[i][j];
	//	}
	//	cout << endl;
	//}

	for (auto i= result_vec.rbegin(); i!=result_vec.rend(); ++i)
	{
		m_enemy_load.emplace_back(i->second * REAL_DISTANCE - m_zero_position.x, 300.0f, i->first * REAL_DISTANCE - m_zero_position.z);
	}
}

Vec2 Astar::GetDestination(const Vector3& src, const Vector3& dst)
{
	Vec2 destination;
	m_zero_position=Vector3{ src.x - REAL_DISTANCE * m_start_pos.first,0.0f,src.z - REAL_DISTANCE * m_start_pos.second };
	Vector3 dst_position{ (dst.x - m_zero_position.x) / REAL_DISTANCE,0.0f,(dst.z - m_zero_position.z) / REAL_DISTANCE };
	destination.first = round(dst_position.z);
	destination.second = round(dst_position.x);
	if (destination.first > 40)destination.first = 40;
	if (destination.second > 40)destination.second = 40;
	return destination;
}