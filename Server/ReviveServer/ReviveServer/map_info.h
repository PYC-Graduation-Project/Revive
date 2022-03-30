#pragma once
#include"vec3.h"
#include<string>

class MapInfo
{
public:
	MapInfo(std::string name, Vector3 pos, int mesh_count, int col_count)
		:name(name), position(pos), mesh_count(mesh_count), collision_count(col_count)
	{

	}
	MapInfo() = default;
	~MapInfo() = default;
	std::string name;
	Vector3 position;
	int mesh_count;
	int collision_count;
};