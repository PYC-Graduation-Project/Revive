#include "stdafx.h"
#include "client/object/actor/static_mesh_actor.h"
#include "client/object/component/mesh/static_mesh_component.h"

namespace client_fw
{
	StaticMeshActor::StaticMeshActor(eMobilityState mobility, const std::string& static_mesh_path)
		: Actor(mobility), m_mesh_path(static_mesh_path)
	{
	}

	void StaticMeshActor::Initialize()
	{
		auto mesh_comp = CreateSPtr<StaticMeshComponent>();
		mesh_comp->SetMesh(m_mesh_path);
		AttachComponent(mesh_comp);
		m_static_mesh_component = mesh_comp;
	}
}
