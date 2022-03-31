#include "stdafx.h"
#include "client/asset/animation/animation_sequence.h"
#include "client/object/component/mesh/skeletal_mesh_component.h"
#include "client/asset/core/asset_store.h"
#include "client/asset/mesh/mesh.h"

namespace client_fw
{
	SkeletalMeshComponent::SkeletalMeshComponent(const std::string& name, const std::string& draw_shader_name)
		:MeshComponent(name, draw_shader_name)
	{
		m_animation_controller = CreateSPtr<AnimationController>();
	}

	bool SkeletalMeshComponent::Initialize()
	{
		m_animation_controller->Initialize();
		return MeshComponent::Initialize();
	}

	void SkeletalMeshComponent::Update(float delta_time)
	{
		if (m_is_playing)
		{
			m_animation_controller->AnimToPlay(delta_time, m_looping);
			GetSkeletalMesh()->GetSkeleton()->UpdateToParent(mat4::IDENTITY);
			m_animation_controller->CopyBoneTransformData();
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
		m_animation_controller->SetMeshPath(file_path);

		auto& skeletal_mesh = GetSkeletalMesh();
		
		m_animation_controller->SetBoneData(skeletal_mesh->GetBoneData(), skeletal_mesh->GetSkeleton());
		skeletal_mesh->GetSkeleton()->UpdateToParent(mat4::IDENTITY);
		return true;
	}

	void SkeletalMeshComponent::SetAnimation(const std::string& animation_name)
	{
		m_animation_name = animation_name;

		if (animation_name.compare("Null") == 0)
			SetIsPlaying(false);
		else if (animation_name.compare("Null") != 0)
		{
			m_animation_controller->SetAnimationName(animation_name);
			m_animation_controller->SetAnimation(GetSkeletalMesh()->GetSkeleton());
		}
	}

	SPtr<SkeletalMeshComponent> SkeletalMeshComponent::SharedFromThis()
	{
		return std::static_pointer_cast<SkeletalMeshComponent>(shared_from_this());
	}
}