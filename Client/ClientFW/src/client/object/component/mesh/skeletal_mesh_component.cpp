#include "stdafx.h"
#include "client/asset/animation/animation_sequence.h"
#include "client/object/component/mesh/skeletal_mesh_component.h"
#include "client/object/actor/core/actor.h"
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
		m_animation_controller->CopyBoneTransformData();

		return true;
	}
	void SkeletalMeshComponent::SetAnimation(const std::string& animation_name,bool looping)
	{
		m_animation_name = animation_name;

		if (animation_name.compare("Null") == 0)
			SetIsPlaying(false);
		else if (animation_name.compare("Null") != 0)
		{
			m_animation_controller->SetAnimationName(animation_name);
			m_animation_controller->SetAnimation(GetSkeletalMesh()->GetSkeleton());
			if (looping == false) m_animation_controller->Initialize();
			SetLooping(looping);
		}
	}

	const Vec3 SkeletalMeshComponent::GetSocketWorldPosition(const std::string& socket_name)
	{
		const auto& socket_world_matrix = GetSocketWorldMatrix(socket_name);
		return Vec3{ socket_world_matrix._41,socket_world_matrix._42,socket_world_matrix._43};
	}

	const Mat4 SkeletalMeshComponent::GetSocketWorldMatrix(const std::string& socket_name)
	{
		GetSkeletalMesh()->GetSkeleton()->UpdateToParent(mat4::IDENTITY);
		Mat4 socket_matrix = GetSkeletalMesh()->GetSkeleton()->FindBone(socket_name)->GetWorld();
		Mat4 mesh_matrix = mat4::CreateScale(GetScale());
		mesh_matrix *= mat4::CreateRotationFromQuaternion(GetLocalRotation());
		mesh_matrix *= mat4::CreateTranslation(GetLocalPosition());
		//socket_matrix *= m_owner.lock()->GetWorldMatrix();
		return socket_matrix * mesh_matrix * m_owner.lock()->GetWorldMatrix();
	}

	const Quaternion SkeletalMeshComponent::GetSocketWorldRotation(const std::string& socket_name)
	{
		GetSkeletalMesh()->GetSkeleton()->UpdateToParent(mat4::IDENTITY);
		Mat4 socket_matrix = GetSkeletalMesh()->GetSkeleton()->FindBone(socket_name)->GetWorld();
		Quaternion socket_rotation;
		XMStoreFloat4(&socket_rotation, XMQuaternionRotationMatrix(XMLoadFloat4x4(&socket_matrix)));

		return socket_rotation * GetLocalRotation() * m_owner.lock()->GetRotation();
	}

	SPtr<SkeletalMeshComponent> SkeletalMeshComponent::SharedFromThis()
	{
		return std::static_pointer_cast<SkeletalMeshComponent>(shared_from_this());
	}
}