#include"pch.h"
#include"map_loader.h"
#include<fstream>
#include<sstream>
#include"define.h"
using namespace std;

void MapLoader::LoadMap(const std::string& path)
{
	ifstream map_file(path, ifstream::binary);
	if (!map_file)
	{
		cout << "맵 파일 불러오기 실패" << endl;
		return;
	}

	stringstream ss;
	string line;
	string prefix;
	string temp_name;
	UINT temp_mesh_count, temp_col_count;
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
			break;
		}
		case HashCode("BoxCollision"):
		{
			break;
		}
		case HashCode("FilePath"): {
			break;
		}
		case HashCode("Position"): {
			break;
		}
		case HashCode("Rotation"): {

			break;
		}
		case HashCode("Scale"): {

			break;
		}
		}
	}
}
