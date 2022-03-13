#include "stdafx.h"
#include "client/object/component/mesh/static_mesh_component.h"
#include "client/asset/core/asset_store.h"
#include "client/asset/mesh/mesh.h"
#include "client/renderer/core/render.h"
#include "client/physics/collision/collisioner/static_mesh_collisioner.h"

namespace client_fw
{
	StaticMeshComponent::StaticMeshComponent(const std::string& name)
		: MeshComponent(name, Render::ConvertShaderType(eShaderType::kOpaqueMesh))
	{
		if (m_collisioner == nullptr)
			m_collisioner = CreateUPtr<StaticMeshCollisioner>();
	}

	bool StaticMeshComponent::Initialize()
	{
		return MeshComponent::Initialize();
	}

	void StaticMeshComponent::Shutdown()
	{
		MeshComponent::Shutdown();
	}

	SPtr<StaticMesh> StaticMeshComponent::GetStaticMesh() const
	{
		return std::static_pointer_cast<StaticMesh>(m_mesh);
	}

	bool StaticMeshComponent::SetMesh(const std::string& file_path)
	{
		m_mesh = std::dynamic_pointer_cast<StaticMesh>(AssetStore::LoadMesh(file_path));
		if (m_mesh == nullptr)
		{
			LOG_ERROR("Could not cast Mesh[{0}] to StaticMesh", file_path);
			return false;
		}
		return true;
	}

	SPtr<StaticMeshComponent> StaticMeshComponent::SharedFromThis()
	{
		return std::static_pointer_cast<StaticMeshComponent>(shared_from_this());
	}
}
