#include "pch.h"
#include "map_manager.h"

#include<fstream>
#include<sstream>
#include<vector>
using namespace std;
void MapManager::LoadMap(const std::string& path)
{
	ifstream map_file(path, ifstream::binary);
	if (!map_file)
	{
		cout << "맵 파일 불러오기 실패" << endl;
		return;
	}
	vector<MapInfo>actor_info_data;
	actor_info_data.reserve(1000);
	stringstream ss;
	string line;
	string prefix;
	string temp_name;
	UINT temp_mesh_count, temp_col_count;
	Vector3 temp_pos;
	std::vector<Vector3> scales;
	std::vector<Vector3> collision_centers;
	std::vector<Vector3> collision_extents;
	std::vector<Vector3> positions;

	while (getline(map_file, line))
	{
		ss.clear();
		prefix.clear();
		ss.str(line);
		ss >> prefix;
		switch (HashCode(prefix.c_str()))
		{
		case HashCode("ActorName"):
		{
			ss >> temp_name >> temp_mesh_count >> temp_col_count;
			ss >> temp_pos.x >> temp_pos.y >> temp_pos.z;
			actor_info_data.emplace_back(temp_name, temp_pos,
				temp_mesh_count, temp_col_count);
			break;
		}
		case HashCode("BoxCollision"):
		{
			ss >> temp_name;//name vector info
			ss >> temp_name >> temp_pos.x >> temp_pos.y >> temp_pos.z;
			collision_centers.emplace_back(std::move(temp_pos));
			ss >> temp_name >> temp_pos.x >> temp_pos.y >> temp_pos.z;
			collision_extents.emplace_back(std::move(temp_pos));
			break;
		}
		case HashCode("FilePath"): {
			break;
		}
		case HashCode("Position"): {
			ss >> temp_pos.x >> temp_pos.y >> temp_pos.z;
			positions.emplace_back(std::move(temp_pos));
			break;
		}
		case HashCode("Rotation"): {

			break;
		}
		case HashCode("Scale"): {
			ss >> temp_pos.x >> temp_pos.y >> temp_pos.z;
			scales.emplace_back(std::move(temp_pos));
			break;
		}
		}
	}
	int col_index{ 0 };
	for (auto& act_info : actor_info_data)
	{
		if (act_info.collision_count <= 0)
			continue;
		switch (HashCode(act_info.name.c_str()))
		{
			//물체들
		case HashCode("Base"):
		{
			for (int i = col_index; i < col_index + act_info.collision_count; ++i)
			{
				//map_obj_manager 만들기
				Vector3 pos{ act_info.position + collision_centers[i] };
				
				m_map_objects.emplace_back(i, pos, collision_extents[i], true,OBJ_TYPE::OT_BASE);

			}
			break;
		}
		case HashCode("Fence"):
		case HashCode("Wall"):
		{
			for (int i = col_index; i < col_index + act_info.collision_count; ++i)
			{
				//map_obj_manager 만들기
				Vector3 pos{ act_info.position + collision_centers[i] };
				m_map_objects.emplace_back(i, pos, collision_extents[i], true,OBJ_TYPE::OT_MAPOBJ);
				
			}
		}
		break;
		//영역들
		case HashCode("ActivityArea"):
		{
			for (int i = col_index; i < col_index + act_info.collision_count; ++i)
			{
				//map_obj_manager 만들기

				Vector3 pos{ act_info.position + collision_centers[i] };
				m_map_objects.emplace_back(i, pos, collision_extents[i], false, OBJ_TYPE::OT_ACTIViTY_AREA);
			}
			break;
		}
		case HashCode("SpawnArea"):
		{
			for (int i = col_index; i < col_index + act_info.collision_count; ++i)
			{
				//map_obj_manager 만들기

				Vector3 pos{ act_info.position + collision_centers[i] };
				m_map_objects.emplace_back(i, pos, collision_extents[i], false,OBJ_TYPE::OT_SPAWN_AREA);
			}
		}
		break;
		}
		col_index += act_info.collision_count;
		//MapObj추가
	}
	BlockTileMap();
}

void MapManager::BlockTileMap()
{
	for (auto& map_obj : m_map_objects)
	{
		for (int i = 0; i < 36; ++i)
		{
			for (int j = 0; j < 16; ++j)
			{
				if (m_tile_map[i][j].type == MAP_OBJ_TYPE::NONE)
				{
					if (map_obj.GetMinPos().x <= m_tile_map[i][j].x && map_obj.GetMaxPos().x >= m_tile_map[i][j].x &&
						map_obj.GetMinPos().z <= m_tile_map[i][j].z && map_obj.GetMaxPos().z >= m_tile_map[i][j].z)
					{
						if (true == map_obj.GetIsBlocked())
							m_tile_map[i][j].type = MAP_OBJ_TYPE::BLOCK;
						else
							m_tile_map[i][j].type = MAP_OBJ_TYPE::UNBLOCK;
					}
				}
			}
		}
	}
	//for (int i = 0; i < 36; ++i)
	//{
	//	for (int j = 0; j < 16; ++j)
	//	{
	//		if (m_tile_map[i][j].type == MAP_OBJ_TYPE::BLOCK)
	//		{
	//			cout << "1";
	//		}
	//		else if (m_tile_map[i][j].type == MAP_OBJ_TYPE::UNBLOCK)
	//		{
	//			cout << "2";
	//		}
	//		else
	//		{
	//			cout << "0";
	//		}
	//	}
	//	cout << endl;
	//}
}



bool MapManager::CheckCollision(const Vector3& obj_pos)
{
	for (auto& map_obj : m_map_objects)
	{
		if (map_obj.GetIsBlocked() == false)continue;
		if (map_obj.GetMinPos().x <= obj_pos.x && map_obj.GetMaxPos().x >= obj_pos.x &&
			map_obj.GetMinPos().z <= obj_pos.z && map_obj.GetMaxPos().z >= obj_pos.z)
		{
			return true;
		}
	}
	return false;
}
