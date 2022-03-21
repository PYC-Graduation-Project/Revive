#include <sstream>
#include <fstream>
#include <include/client_core.h>
#include <client/object/component/mesh/static_mesh_component.h>
#include "object/map/map_loader.h"
#include "object/actor/ground.h"
namespace revive
{
	struct ActorInfo
	{
		std::string name;
		UINT mesh_count;
	};

	std::vector<SPtr<Actor>> MapLoader::LoadMap(const std::string& path, eMapLoadType type)
	{
		std::ifstream map_file(path,std::ifstream::binary);

		if (map_file.is_open() == false)
		{
			LOG_ERROR("Could not find path : [{0}]", path);
			//return;
		}



		std::stringstream ss;
		std::string line;
		std::string prefix;
		Vec3 temp_vec;
		Quaternion temp_quat;
		std::string temp_string;
		UINT temp_uint;
		std::map<std::string, int> actor_count;

		std::vector<SPtr<Actor>> actors;

		std::vector<SPtr<StaticMeshComponent>> components;
		std::vector<ActorInfo> actor_info_data;
		std::vector<std::string> file_paths;
		std::vector<int> mesh_count_data;
		std::vector<Vec3> positions;
		std::vector<Quaternion> rotations;
		std::vector<Vec3> scales;

		while (std::getline(map_file, line))
		{
			ss.clear();
			prefix.clear();
			ss.str(line);
			ss >> prefix;
			switch (HashCode(prefix.c_str()))
			{
			
			case HashCode("ActorName"):
			{
				ss >> temp_string >> temp_uint;
				actor_info_data.push_back({ temp_string ,temp_uint });

				//auto iter = std::find_if(actor_info_data.cbegin(), actor_info_data.cend(),
				//	[temp_string](const ActorInfo& data) {
				//	return data.name == temp_string;
				//});

				//if (iter == actor_info_data.cend())
				//{
				//	//combine_data_index = static_cast<UINT>(combine_data.size()) - 1;
				//}
				//else
				//{
				//	//combine_data_index = static_cast<UINT>(std::distance(combine_data.cbegin(), iter));
				//}
				if (actor_count.find(temp_string) == actor_count.end())
				{
					actor_count.insert({ temp_string,1 });
				}
				else ++actor_count[temp_string];
			}
				break;
			case HashCode("FilePath"):
				ss >> temp_string;
				file_paths.emplace_back(std::move(temp_string));
				break;
			case HashCode("MeshCount"):
				ss >> temp_uint;
				mesh_count_data.emplace_back(std::move(temp_uint));
				break;
			case HashCode("Position"):
				ss >> temp_vec.x >> temp_vec.y >> temp_vec.z;
				positions.emplace_back(std::move(temp_vec));
				break;
			case HashCode("Rotation"):
				ss >> temp_quat.x >> temp_quat.y >> temp_quat.z >> temp_quat.w;
				rotations.emplace_back(std::move(temp_quat));
				break;
			case HashCode("Scale"):
				ss >> temp_vec.x >> temp_vec.y >> temp_vec.z;
				scales.emplace_back(std::move(temp_vec));
				break;
			}
		}
		UINT actor_mesh_index = 0;
		for (auto actor : actor_info_data)
		{
			switch (HashCode(actor.name.c_str())) //클래스 or 액터 이름
			{
			case HashCode("Ground"):
				for (UINT index = 0; index < actor_count[actor.name]; ++index) //액터의 갯수만큼 생성
				{
					//std::vector<Vec3> temp_pos(actor.mesh_count);
					//std::vector<Vec3> temp_scale(actor.mesh_count);

					for (UINT count = actor_mesh_index; count < actor.mesh_count; ++count)
					{
						SPtr<StaticMeshComponent> component = CreateSPtr<StaticMeshComponent>();
						component->SetMesh(file_paths[count]);//메시가 다를 수도있음 같을수도있고
						component->SetLocalPosition(positions[count]);
						component->SetLocalScale(scales[count]);
						components.emplace_back(std::move(component));
					}
					actor_mesh_index += actor.mesh_count;

					SPtr<Ground> ground = CreateSPtr<Ground>(components);
					actors.push_back(ground);
				}
				break;
			}
		}
		
		//for (const auto& actor : actor_count)
		//{
		//	
		//	switch (HashCode(actor.first.c_str())) //클래스 or 액터 이름
		//	{
		//	case HashCode("Ground"):
		//		for (UINT index = 0; index < actor.second; ++index) //액터의 갯수만큼 생성
		//		{
		//			
		//			file_paths

		//		}
		//		break;
		//	}
		//}
		return actors;
	}
}