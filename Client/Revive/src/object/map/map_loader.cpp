#include "object/map/map_loader.h"
#include "object/actor/ground.h"
#include "object/actor/Wall.h"
#include "object/actor/spawn_area.h"
#include "object/actor/Base.h"

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
		float temp_float;
		std::map<std::string, int> actor_count;

		std::vector<SPtr<Actor>> actors;

		std::vector<ActorInfo> actor_info_data;
		std::vector<std::string> file_paths;
		std::vector<Vec3> positions;
		std::vector<Quaternion> rotations;
		std::vector<float> scales;
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
				if (temp_string == "ActivityArea")
				{
					int i = 0;
				}
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
				ss >> temp_float;
				scales.emplace_back(std::move(temp_float));
				break;
			}
		}

		UINT actor_mesh_index = 0;
		UINT actor_collision_index = 0;
		for (auto actor_info : actor_info_data)
		{
			if (type == eMapLoadType::kServer)
			{
				actor_info.mesh_count = 0; //서버는 메시는 필요없으므로 메쉬 갯수를 0으로 만들어버린다.
				if (actor_info.collision_count <= 0) continue; //서버인데 콜리전 정보가 없으면 다음 액터 정보 확인
			}

			SPtr<Actor> actor = nullptr;
			
			switch (HashCode(actor_info.name.c_str())) //클래스 or 액터 이름
			{
			//박스 컴포넌트 없는 애들
			//서버면 여기는 안올거임
			case HashCode("Ground"):
			case HashCode("Bridge"):
			{
				actor = CreateSPtr<Ground>(
					CreateStaticMeshComponents(actor_mesh_index, actor_info, file_paths, positions, rotations, scales)
					);
			}
			break;
			//박스 컴포넌트만,혹은 박스랑 스태틱 둘다 있는 애들
			case HashCode("Base"):
			{
				actor = CreateSPtr<Base>(
					CreateStaticMeshComponents(actor_mesh_index, actor_info, file_paths, positions, rotations, scales),
					CreateBoxComponents(actor_collision_index, actor_info, collision_extents, collision_centers)
					);
			}
			break;
			case HashCode("Fence"):
			case HashCode("Wall"):
			{
				if (actor_info.mesh_count > 0 && actor_info.collision_count > 0) //박스랑 메시둘다 있다 서버는 박스만 넣어야함
				{
						actor = CreateSPtr<Wall>(
							CreateStaticMeshComponents(actor_mesh_index, actor_info, file_paths, positions, rotations, scales),
							CreateBoxComponents(actor_collision_index, actor_info, collision_extents, collision_centers)
							);
				}
				else if (actor_info.mesh_count > 0) //메시만 있다. 서버는 무시됨
				{
					actor = CreateSPtr<Wall>(
						CreateStaticMeshComponents(actor_mesh_index, actor_info, file_paths, positions, rotations, scales)
						);
				}
				else if (actor_info.collision_count > 0) //박스만있다.
				{
					actor = CreateSPtr<Wall>(
						CreateBoxComponents(actor_collision_index, actor_info, collision_extents, collision_centers)
						);
				}
			}
			break;
			//박스 컴포넌트만 있는 애들
			case HashCode("ActivityArea"):
			case HashCode("SpawnArea"):
			{
				if (actor_info.collision_count > 0)
				{
					actor = CreateSPtr<SpawnArea>(
						CreateBoxComponents(actor_collision_index, actor_info, collision_extents, collision_centers)
						);
				}
			}
			break;
			}
			actor_mesh_index += actor_info.mesh_count;
			actor_collision_index += actor_info.collision_count;
			actor->SetName(actor_info.name);
			actor->SetPosition(actor_info.position);
			actors.emplace_back(std::move(actor));
		}
	
		return actors;
	}
	std::vector<SPtr<BoxComponent>> MapLoader::CreateBoxComponents(
		const UINT& actor_collision_index, const ActorInfo& actor_info,
		const std::vector<Vec3>& collision_extents, const std::vector<Vec3>& collision_centers)
	{
		std::vector<SPtr<BoxComponent>> box_components;

		for (UINT count = actor_collision_index; count < actor_collision_index + actor_info.collision_count; ++count)
		{
			SPtr<BoxComponent> box_component = CreateSPtr<BoxComponent>(collision_extents[count], actor_info.name + " box component");
			box_component->SetLocalPosition(collision_centers[count]);
			box_component->SetCollisionInfo(true, false, "default", { "default" }, true);
			box_components.emplace_back(std::move(box_component));
		}
		return box_components;
	}


	std::vector<SPtr<StaticMeshComponent>> MapLoader::CreateStaticMeshComponents(
		const UINT& actor_mesh_index, const ActorInfo& actor_info, const std::vector<std::string>& file_paths, 
		const std::vector<Vec3>& positions, const std::vector<Quaternion>& rotations, const std::vector<float>& scales)
	{
		std::vector<SPtr<StaticMeshComponent>> components;

		for (UINT count = actor_mesh_index; count < actor_mesh_index + actor_info.mesh_count; ++count)
		{
			SPtr<StaticMeshComponent> component = CreateSPtr<StaticMeshComponent>();
			component->SetMesh(file_paths[count]);//메시가 다를 수도있음 같을수도있고
			component->SetLocalPosition(positions[count]);
			component->SetLocalRotation(rotations[count]);
			component->SetLocalScale(scales[count]);
			components.emplace_back(std::move(component));
		}
		return components;
	}
	
	
}