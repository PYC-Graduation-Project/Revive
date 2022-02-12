#include "stdafx.h"
#include "client/object/component/core/scene_component.h"
#include "client/object/actor/core/actor.h"
#include "client/util/octree/octree_manager.h"
#include "client/physics/collision/collisioner.h"

namespace client_fw
{
	SceneComponent::SceneComponent(const std::string& name, int update_order)
		: Component(name, update_order)
		, m_local_position(vec3::ZERO), m_local_scale(Vec3(1.0f, 1.0f, 1.0f))
	{
	}

	SceneComponent::~SceneComponent()
	{
	}

	bool SceneComponent::InitializeComponent()
	{
		m_collisioner = std::move(CreateCollisioner());
		UpdateLocalMatrix();
		bool ret = Initialize();
		return ret;
	}

	void SceneComponent::ShutdownComponent()
	{
		UnregisterToCollsionOctree();
		Shutdown();
	}

	void SceneComponent::UpdateComponent(float delta_time)
	{
		m_is_updated_world_matrix = false;

		UpdateLocalMatrix();

		Update(delta_time);
	}

	void SceneComponent::UpdateWorldMatrix()
	{
		const auto& owner = m_owner.lock();
		if (owner != nullptr)
		{
			const auto& world = owner->GetWorldMatrix();
			m_world_matrix = m_local_matrix * world;
			m_world_position = vec3::TransformCoord(m_local_position, world);
			m_world_rotation *= owner->GetRotation();
			m_world_scale *= owner->GetScale();
			UpdateOrientedBox();

			m_is_updated_world_matrix = true;

			RegisterToCollisionOctree();
		}
	}

	void SceneComponent::UpdateLocalMatrix()
	{
		if (m_update_local_matrix)
		{
			m_local_matrix = mat4::CreateScale(m_local_scale);
			m_local_matrix *= mat4::CreateRotationFromQuaternion(m_local_rotation);
			m_local_matrix *= mat4::CreateTranslation(m_local_position);
			m_update_local_matrix = false;

			UpdateWorldMatrix();
		}
	}

	void SceneComponent::UpdateOrientedBox()
	{
	}

	void SceneComponent::RegisterToCollisionOctree()
	{
		if (m_collisioner != nullptr)
		{
			if (m_collisioner->GetCollisionInfo().preset != eCollisionPreset::kNoCollision)
				CollisionOctreeManager::GetOctreeManager().ReregisterSceneComponent(shared_from_this());
		}
	}

	void SceneComponent::UnregisterToCollsionOctree()
	{
		if (m_collisioner != nullptr)
		{
			if (m_collisioner->GetCollisionInfo().preset != eCollisionPreset::kNoCollision)
				CollisionOctreeManager::GetOctreeManager().UnregisterSceneComponent(shared_from_this());
		}
	}

	void SceneComponent::SetLocalPosition(const Vec3& pos)
	{
		m_local_position = pos;
		m_update_local_matrix = true;
	}

	void SceneComponent::SetLocalRotation(const Quaternion& rot)
	{
		m_local_rotation = rot;
		m_update_local_matrix = true;
	}

	void SceneComponent::SetLocalRotation(float pitch, float yaw, float roll)
	{
		m_local_rotation = quat::CreateQuaternionFromRollPitchYaw(pitch, yaw, roll);
		m_update_local_matrix = true;
	}

	void SceneComponent::SetLocalScale(const Vec3& scale)
	{
		m_local_scale = scale;
		m_update_local_matrix = true;
	}

	void SceneComponent::SetLocalScale(float scale)
	{
		m_local_scale = Vec3(scale, scale, scale);
		m_update_local_matrix = true;
	}

	UPtr<Collisioner> SceneComponent::CreateCollisioner()
	{
		return nullptr;
	}

	void SceneComponent::AddCollisionTreeNode(const WPtr<CollisionTreeNode>& tree_node)
	{
		m_collision_tree_node.push_back(tree_node);
	}
}
