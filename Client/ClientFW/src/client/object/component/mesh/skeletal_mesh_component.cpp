#include "stdafx.h"
#include "client/asset/animation/animation_sequence.h"
#include "client/object/component/mesh/skeletal_mesh_component.h"
#include "client/asset/core/asset_store.h"
#include "client/asset/mesh/mesh.h"
#include "client/asset/bone/skeleton.h"
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
	void SkeletalMeshComponent::Update(float delta_time)
	{
		if (m_anim_seq && GetSkeletalMesh()->GetSkeleton())
		{
			m_anim_seq->AnimToPlay(delta_time,m_looping);
			
			GetSkeletalMesh()->GetSkeleton()->UpdateToParent(mat4::IDENTITY);
			//GetSkeletalMesh()->GetSkeleton()->UpdateToParent(GetWorldMatrix());
		}
		
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
	void SkeletalMeshComponent::SetAnimation(const std::string& path)
	{
		m_anim_seq = AssetStore::LoadAnimation(path);
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