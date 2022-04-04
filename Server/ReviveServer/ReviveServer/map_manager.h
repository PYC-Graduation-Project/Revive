#pragma once
#include<vector>
#include"map_object.h"
#include"map_info.h"
#include"define.h"
class MapManager
{
public:
	MapManager() {
	
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
private:
	std::vector<MapObj>m_map_objects;

};

