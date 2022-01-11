#include "stdafx.h"
#include "client/object/component/mesh/static_mesh_component.h"
#include "client/renderer/core/render.h"

namespace client_fw
{
	StaticMeshComponent::StaticMeshComponent(const std::string& name)
		: MeshComponent(name, Render::ConvertShaderType(eShaderType::kDefault))
	{
	}

	bool StaticMeshComponent::Initialize()
	{
		return MeshComponent::Initialize();
	}

	void StaticMeshComponent::Shutdown()
	{
		MeshComponent::Shutdown();
	}
}
