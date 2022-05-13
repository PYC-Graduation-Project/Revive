#include <include/client_core.h>
#include "object/actor/wall.h"

namespace revive
{
	Wall::Wall()
		:Actor(eMobilityState::kStatic)
	{
	}
	Wall::Wall(const std::vector<SPtr<StaticMeshComponent>>& static_mesh_components, const std::vector<SPtr<BoxComponent>>& box_components)
	{
		m_static_mesh_components = static_mesh_components;
		m_box_components = box_components;
	}
	Wall::Wall(const std::vector<SPtr<StaticMeshComponent>>& static_mesh_components)
		: Actor(eMobilityState::kStatic)
	{
		m_static_mesh_components = static_mesh_components;
	}
	Wall::Wall(const std::vector<SPtr<BoxComponent>>& box_components)
		: Actor(eMobilityState::kStatic)
	{
		m_box_components = box_components;
	}
	bool Wall::Initialize()
	{
		bool ret = true;
		
		for (auto& static_mesh_component : m_static_mesh_components)
			ret &= AttachComponent(static_mesh_component);
		
		for (auto& box_component : m_box_components)
		{
			box_component->SetCollisionInfo(true, true, "wall", { "player","player camera"}, true);
			ret &= AttachComponent(box_component);
		}
		
		return true;
	}
	void Wall::Shutdown()
	{
		for (auto& static_mesh_component : m_static_mesh_components)
		{
			static_mesh_component = nullptr;
		}
		for (auto& box_component : m_box_components)
		{
			box_component = nullptr;
		}
	}
	void Wall::Update(float delta_time)
	{
		/*for (auto& component : m_box_components)
		{
			LOG_INFO("{0} : {1}", component->GetName(), component->GetWorldMatrix());
		}*/
	}
}