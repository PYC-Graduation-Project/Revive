#pragma once
#include<string>
#include<vector>
#include<unordered_map>
class MapObj;
class ActorInfo
{
public:
	ActorInfo(std::string name, Vector3 pos, UINT mesh_count, UINT col_count)
		:name(name),position(pos),mesh_count(mesh_count),collision_count(col_count)
	{

	}
	ActorInfo() = default;
	~ActorInfo()=default;
	std::string name;
	Vector3 position;
	UINT mesh_count;
	UINT collision_count;
};
class MapLoader
{
public:
	MapLoader() = default;
	~MapLoader() = default;

	void LoadMap(const std::string& path);

//private:
	std::unordered_map<int, MapObj*>m_map_objects;
};