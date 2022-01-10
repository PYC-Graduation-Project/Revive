#include "stdafx.h"
#include "client/object/component/mesh/static_mesh_component.h"

namespace client_fw
{
	StaticMeshComponent::StaticMeshComponent(const std::string& name)
	{
	}

	void StaticMeshComponent::Initialize()
	{
		MeshComponent::Initialize();
	}

	void StaticMeshComponent::Shutdown()
	{
		MeshComponent::Shutdown();
	}
}
