#include "stdafx.h"
#include "client/object/component/core/scene_component.h"
#include "client/object/actor/core/actor.h"

namespace client_fw
{
	SceneComponent::SceneComponent(const std::string& name, int update_order)
		: Component(name, update_order)
		, m_local_position(vec3::ZERO), m_local_scale(Vec3(1.0f, 1.0f, 1.0f))
	{
	}

	bool SceneComponent::InitializeComponent()
	{
		UpdateLocalMatrix();
		bool ret = Initialize();
		return ret;
	}

	void SceneComponent::Update(float delta_time)
	{
		m_is_updated_world_matrix = false;

		UpdateLocalMatrix();
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
			m_is_updated_world_matrix = true;
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
}
