#pragma once
#include <sstream>
#include <fstream>
#include <include/client_core.h>
#include <client/object/actor/core/actor.h>
#include <client/object/component/mesh/static_mesh_component.h>
#include <client/object/component/render/box_component.h>

namespace revive
{
	using namespace client_fw;

	struct ActorInfo;

	enum class eMapLoadType
	{
		kClient,kServer,
	};

	class MapLoader
	{
	public:
		MapLoader() = default;
		~MapLoader() = default;

		std::vector<SPtr<Actor>> LoadMap(const std::string& path, eMapLoadType type);
	private:
		std::vector<SPtr<StaticMeshComponent>> CreateStaticMeshComponents(
			const UINT& actor_mesh_index, const ActorInfo& actor_info, const std::vector<std::string>& file_paths,
			const std::vector<Vec3>& positions, const std::vector<Quaternion>& rotations, const std::vector<Vec3>& scales);
		std::vector<SPtr<BoxComponent>> CreateBoxComponents(const UINT& actor_collision_index, const ActorInfo& actor_info, 
			const std::vector<Vec3>& collision_extents, const std::vector<Vec3>& collision_centers);
	};
}