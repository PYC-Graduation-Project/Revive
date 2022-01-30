#include "stdafx.h"
#include "client/object/actor/static_mesh_actor.h"
#include "client/object/component/mesh/static_mesh_component.h"

namespace client_fw
{
	StaticMeshActor::StaticMeshActor(eMobilityState mobility, const std::string& static_mesh_path)
		: Actor(mobility), m_mesh_path(static_mesh_path)
	{
		m_static_mesh_component = CreateSPtr<StaticMeshComponent>();
	}

	bool StaticMeshActor::Initialize()
	{
		m_static_mesh_component->SetMesh(m_mesh_path);
		return AttachComponent(m_static_mesh_component);
	}

	void StaticMeshActor::Shutdown()
	{
		m_static_mesh_component = nullptr;
	}
}
