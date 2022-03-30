#pragma once
#include<vector>
class MapInfo;
class MapObj;
class MapManager
{
public:
	MapManager()=default;
	~MapManager()=default;
	void LoadMap(const std::string& path);
private:
	std::vector<MapObj>m_map_objects;
};

