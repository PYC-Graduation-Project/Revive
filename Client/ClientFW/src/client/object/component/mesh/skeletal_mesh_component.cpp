#include "stdafx.h"
#include "client/object/component/mesh/skeletal_mesh_component.h"
#include "client/asset/core/asset_store.h"
#include "client/asset/mesh/mesh.h"
#include "client/renderer/core/render.h"
#include "client/physics/collision/collisioner/static_mesh_collisioner.h"

namespace client_fw
{
	SkeletalMeshComponent::SkeletalMeshComponent(const std::string& name)
		:MeshComponent(name, Render::ConvertShaderType(eShaderType::kOpaqueMesh))
	{

	}
	bool SkeletalMeshComponent::Initialize()
	{
		return MeshComponent::Initialize();
	}
	void SkeletalMeshComponent::Shutdown()
	{
		MeshComponent::Shutdown();
	}
	SPtr<SkeletalMesh> SkeletalMeshComponent::GetSkeletalMesh() const
	{
		return std::static_pointer_cast<SkeletalMesh>(m_mesh);
	}
	bool SkeletalMeshComponent::SetMesh(const std::string& file_path)
	{
		m_mesh = std::dynamic_pointer_cast<SkeletalMesh>(AssetStore::LoadMesh(file_path));
		if (m_mesh == nullptr)
		{
			LOG_ERROR("Could not cast Mesh[{0}] to SkeletalMesh", file_path);
			return false;
		}
		return true;
	}
	UPtr<Collisioner> SkeletalMeshComponent::CreateCollisioner()
	{
		return nullptr;

	}
	SPtr<SkeletalMeshComponent> SkeletalMeshComponent::SharedFromThis()
	{
		return std::static_pointer_cast<SkeletalMeshComponent>(shared_from_this());

	}
}