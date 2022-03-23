#include <include/client_core.h>
#include <client/object/component/mesh/static_mesh_component.h>
#include "gravity_actor.h"

namespace event_test
{
	GravityActor::GravityActor(const std::string& path)
		: StaticMeshActor(eMobilityState::kMovable, path)
	{
	}

	bool GravityActor::Initialize()
	{
		bool ret = StaticMeshActor::Initialize();
		m_static_mesh_component->SetPhysics(true);
		return ret;
	}

	void GravityActor::Shutdown()
	{
	}

	void GravityActor::Update(float delta_time)
	{
	}
}