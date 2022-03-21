#include <sstream>
#include <fstream>
#include <include/client_core.h>
#include <client/object/component/mesh/static_mesh_component.h>
#include <client/object/component/render/box_component.h>
#include "object/map/map_loader.h"
#include "object/actor/ground.h"
#include "object/actor/Wall.h"

namespace revive
{
	struct ActorInfo
	{
		std::string name;
		Vec3 position;
		UINT mesh_count;
		UINT collision_count;
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
		UINT temp_uint, temp_uint2;
		std::map<std::string, int> actor_count;

		std::vector<SPtr<Actor>> actors;

		std::vector<SPtr<StaticMeshComponent>> components;
		std::vector<SPtr<BoxComponent>> box_components;
		std::vector<ActorInfo> actor_info_data;
		std::vector<std::string> file_paths;
		std::vector<Vec3> positions;
		std::vector<Quaternion> rotations;
		std::vector<Vec3> scales;
		std::vector<Vec3> collision_centers;
		std::vector<Vec3> collision_extents;

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
				ss >> temp_string >> temp_uint >> temp_uint2;
				ss >> temp_vec.x >> temp_vec.y >> temp_vec.z;
				actor_info_data.push_back({ temp_string ,temp_vec, temp_uint,temp_uint2 });

				if (actor_count.find(temp_string) == actor_count.end())
				{
					actor_count.insert({ temp_string,1 });
				}
				else ++actor_count[temp_string];
			}
				break;
			case HashCode("BoxCollision"):
			{
				ss >> temp_string ;//name vector info
				ss >> temp_string >> temp_vec.x >> temp_vec.y >> temp_vec.z;
				collision_centers.emplace_back(std::move(temp_vec));
				ss >> temp_string >> temp_vec.x >> temp_vec.y >> temp_vec.z;
				collision_extents.emplace_back(std::move(temp_vec));
				
			}
				break;
			case HashCode("FilePath"):
				ss >> temp_string;
				file_paths.emplace_back(std::move(temp_string));
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
		UINT actor_collision_index = 0;
		for (auto actor_info : actor_info_data)
		{
			components.clear();
			switch (HashCode(actor_info.name.c_str())) //클래스 or 액터 이름
			{
			case HashCode("Ground"):
			case HashCode("Bridge"):
			{
				
				for (UINT count = actor_mesh_index; count < actor_mesh_index + actor_info.mesh_count; ++count)
				{
					SPtr<StaticMeshComponent> component = CreateSPtr<StaticMeshComponent>();
					component->SetMesh(file_paths[count]);//메시가 다를 수도있음 같을수도있고
					component->SetLocalPosition(positions[count]);
					component->SetLocalRotation(rotations[count]);
					component->SetLocalScale(scales[count]);
					components.emplace_back(std::move(component));
					//components.push_back({ component });

				}
				auto ground = CreateSPtr<Ground>(components);
				ground->SetPosition(actor_info.position);
				ground->SetName(actor_info.name);
				if (actor_info.name == "Bridge")
				{
					int i = 0;
				}
				actor_mesh_index += actor_info.mesh_count;

				for (UINT count = actor_collision_index; count < actor_collision_index + actor_info.collision_count; ++count)
				{
					//ground->AttachComponent(box_components[count]);
				}
				actor_collision_index += actor_info.collision_count;

				actors.emplace_back(std::move(ground));
			}
			break;
			case HashCode("Fence"):
			case HashCode("Wall"):
			{
				Vec3 scle{ 1,1,1 };
				Quaternion quat{ 0,0,0,1 };
				for (UINT count = actor_mesh_index; count < actor_mesh_index + actor_info.mesh_count; ++count)
				{
					SPtr<StaticMeshComponent> component = CreateSPtr<StaticMeshComponent>();
					component->SetMesh(file_paths[count]);//메시가 다를 수도있음 같을수도있고
					component->SetLocalScale(scales[count]);
					component->SetLocalPosition(positions[count]);
					component->SetLocalRotation(rotations[count]);
					components.push_back({ component });
				}
				for (UINT count = actor_collision_index; count < actor_collision_index + actor_info.collision_count; ++count)
				{
					SPtr<BoxComponent> box_component = CreateSPtr<BoxComponent>(collision_extents[count], actor_info.name);
					box_component->SetLocalPosition(collision_centers[count]);
					box_components.emplace_back(std::move(box_component));
				}
				actor_mesh_index += actor_info.mesh_count;
				actor_collision_index += actor_info.collision_count;

				auto wall = CreateSPtr<Wall>(components);
				wall->SetName(actor_info.name);
				wall->SetPosition(actor_info.position);

				actors.emplace_back(std::move(wall));
				

			}
			break;
			}
		}
	
		return actors;
	}
	
	
}