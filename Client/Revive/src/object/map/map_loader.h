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

	class MapLoader
	{
	public:
		MapLoader() = default;
		~MapLoader() = default;

		std::vector<SPtr<Actor>> LoadMap(const std::string& path);
	private:

	};
}