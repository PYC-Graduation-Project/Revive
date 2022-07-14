#include <include/client_core.h>
#include "object/actor/visual_actor.h"

namespace revive
{
	VisualActor::VisualActor()
		:Actor(eMobilityState::kStatic)
	{
	}
	VisualActor::VisualActor(const SPtr<StaticMeshComponent>& static_mesh_component)
		: Actor(eMobilityState::kStatic)
	{
		m_static_mesh_components.push_back(static_mesh_component);
	}
	VisualActor::VisualActor(const std::vector<SPtr<StaticMeshComponent>>& static_mesh_components, const std::vector<SPtr<BoxComponent>>& box_components)
		: Actor(eMobilityState::kStatic)
	{
		m_static_mesh_components = static_mesh_components;
		m_box_components = box_components;
	}
	VisualActor::VisualActor(const std::vector<SPtr<StaticMeshComponent>>& static_mesh_components)
		: Actor(eMobilityState::kStatic)
	{
		m_static_mesh_components = static_mesh_components;
	}
	VisualActor::VisualActor(const std::vector<SPtr<BoxComponent>>& box_components)
		: Actor(eMobilityState::kStatic)
	{
		m_box_components = box_components;
	}
	bool VisualActor::Initialize()
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
	void VisualActor::Shutdown()
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
	void VisualActor::Update(float delta_time)
	{
		/*for (auto& component : m_box_components)
		{
			LOG_INFO("{0} : {1}", component->GetName(), component->GetWorldMatrix());
		}*/
	}
}