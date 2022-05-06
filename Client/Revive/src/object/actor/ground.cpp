#include <include/client_core.h>
#include "object/actor/ground.h"

namespace revive
{
	Ground::Ground(const std::vector<SPtr<StaticMeshComponent>>& components)
		: Actor(eMobilityState::kStatic)
	{
		m_static_mesh_components = components;
	}

	bool Ground::Initialize()
	{
		bool ret = true;
		for (auto& static_mesh_component : m_static_mesh_components)
		{
			static_mesh_component->SetCollisionInfo(true, true, "ground", { "player camera" },false);
			ret &= AttachComponent(static_mesh_component);
		}
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