#pragma once
#include<vector>
#include"map_object.h"
#include"map_info.h"
#include"define.h"
class MapManager
{
public:
	MapManager() {
		for (int i = 0; i < 36; ++i)
		{
			for (int j = 0; j < 16; ++j)
			{
				m_tile_map[i][j].x = j * 300.f + 150.f;
				m_tile_map[i][j].z = i * 300.f + 150.f;
				m_tile_map[i][j].type = MAP_OBJ_TYPE::NONE;
			}
		}
		m_map_objects.reserve(100);
	};
	~MapManager()=default;
	void LoadMap(const std::string& path);
	void BlockTileMap();
	bool CheckCollision(const Vector3& obj_pos);
	const std::vector<MapObj>& GetMapObjVec() const
	{
		return m_map_objects;
	}
	const MapObj& GetMapObjectByType(OBJ_TYPE type)
	{
		for (auto& obj : m_map_objects)
		{
			if (obj.GetType() == type)
			{
				return obj;
			}
		}
		return MapObj();
	}
private:
	std::vector<MapObj>m_map_objects;
	MapTile m_tile_map[36][16];
};

