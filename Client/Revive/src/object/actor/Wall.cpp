#include <include/client_core.h>
#include "object/actor/wall.h"

namespace revive
{
	Wall::Wall()
		:Actor(eMobilityState::kStatic)
	{
	}
	Wall::Wall(const std::vector<SPtr<StaticMeshComponent>>& components)
		: Actor(eMobilityState::kStatic)
	{
		m_static_mesh_components = components;
	}
	bool Wall::Initialize()
	{
		bool ret = true;

		for (auto& static_mesh_component : m_static_mesh_components)
			ret &= AttachComponent(static_mesh_component);
		return true;
	}
	void Wall::Shutdown()
	{
		for (auto& static_mesh_component : m_static_mesh_components)
		{
			static_mesh_component = nullptr;
		}
	}
	void Wall::Update(float delta_time)
	{
		/*for (auto& component : m_static_mesh_components)
		{
			LOG_INFO("{0} : {1}", component->GetName(), component->GetWorldMatrix());
		}*/
	}
}