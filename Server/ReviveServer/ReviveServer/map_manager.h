#pragma once
#include<vector>
#include"map_object.h"
class MapInfo;
class MapManager
{
public:
	MapManager() {
		//m_map_objects = std::vector<MapObj>();
		m_map_objects.reserve(100);
	};
	~MapManager()=default;
	void LoadMap(const std::string& path);
private:
	std::vector<MapObj>m_map_objects;
};

