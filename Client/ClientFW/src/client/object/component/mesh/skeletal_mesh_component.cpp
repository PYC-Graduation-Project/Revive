#include "stdafx.h"
#include "client/asset/animation/animation_sequence.h"
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
			GetSkeletalMesh()->GetSkeleton()->UpdateToParent(GetWorldMatrix());
			m_animation_controller->CopyAnimationData();
		}
	}
	void SkeletalMeshComponent::Shutdown()
	{
		UnregisterSkeletalMeshComponent();
		MeshComponent::Shutdown();
	}
	bool SkeletalMeshComponent::RegisterSkeletalMeshComponent()
	{
		return Render::RegisterSkeletalMeshComponent(SharedFromThis(),m_draw_shader_name);
	}
	void SkeletalMeshComponent::UnregisterSkeletalMeshComponent()
	{
		return Render::UnregisterSkeletalMeshComponent(SharedFromThis(), m_draw_shader_name);
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
		auto& skeletal_mesh = GetSkeletalMesh();
		
		m_animation_controller->SetBoneData(skeletal_mesh->GetBoneData(), skeletal_mesh->GetSkeleton());
		skeletal_mesh->GetSkeleton()->UpdateToParent(GetWorldMatrix());
		m_animation_controller->CopyAnimationData();

		return true;
	}
	void SkeletalMeshComponent::SetAnimation(const std::string& animation_path,const std::string& animation_name)
	{
		m_animation_name = animation_name;
		m_animation_controller->SetAnimation(animation_path, GetSkeletalMesh()->GetSkeleton());
		m_animation_controller->SetAnimationName(animation_name);
		if (GetIsRegistered() == false)
			SetIsRegistered(RegisterSkeletalMeshComponent());
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