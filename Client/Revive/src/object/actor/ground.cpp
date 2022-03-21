#include <include/client_core.h>
#include "object/actor/ground.h"

namespace revive
{
	Ground::Ground()
		:Actor(eMobilityState::kStatic)
	{
	}
	Ground::Ground(const std::string& path)
		:Actor(eMobilityState::kStatic)
	{
		m_static_mesh_components.resize(5);
		float i = 30;
		for (auto& static_mesh_component : m_static_mesh_components)
		{
			static_mesh_component = CreateSPtr<StaticMeshComponent>();
			static_mesh_component->SetMesh(path);
			//block->SetLocalPosition(Vec3{0.0f, 0.0f, 0.0f+i});
			//block->SetLocalScale(Vec3{10.0f, 10.0f, 10.0f});
			i += 30.0f;
		}
	}
	Ground::Ground(const std::vector<SPtr<StaticMeshComponent>>& components)
		: Actor(eMobilityState::kStatic)
	{
		m_static_mesh_components = components;
	}

	bool Ground::Initialize()
	{
		bool ret = true;
		for (auto& static_mesh_component : m_static_mesh_components)
			ret &= AttachComponent(static_mesh_component);
		
		return ret;
	}
	void Ground::Shutdown()
	{
		for (auto& static_mesh_component : m_static_mesh_components)
		{
			static_mesh_component = nullptr;
		}
	}
	void Ground::Update(float delta_time)
	{
		/*for (auto& block : m_blocks)
		{
			LOG_INFO("Fence visible {0}", block->GetWorldMatrix());
		}*/
		
	}
}