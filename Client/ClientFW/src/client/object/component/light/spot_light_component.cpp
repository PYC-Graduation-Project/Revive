#include "stdafx.h"
#include "client/object/component/light/spot_light_component.h"
#include "client/object/actor/core/actor.h"

namespace client_fw
{
	SpotLightComponent::SpotLightComponent(const std::string& name,
		const std::string& draw_shader_name)
		: LocalLightComponent(eLightType::kSpot, name, draw_shader_name)
	{
	}

	void SpotLightComponent::UpdateWorldMatrix()
	{
		const auto& owner = m_owner.lock();
		if (owner != nullptr)
		{
			float r = m_attenuation_radius;
			const auto& world = owner->GetWorldMatrix();
			m_world_position = vec3::TransformCoord(m_local_position, world);
			m_world_rotation = m_local_rotation * owner->GetRotation();
			m_world_scale = m_local_scale * owner->GetScale();

			m_world_matrix = mat4::CreateScale(Vec3(r, r, r));
			m_world_matrix *= mat4::CreateRotationFromQuaternion(m_world_rotation);
			m_world_matrix *= mat4::CreateTranslation(m_world_position);

			UpdateOrientedBox();

			m_is_updated_world_matrix = true;
		}
	}

	void SpotLightComponent::UpdateLocalMatrix()
	{
		if (m_update_local_matrix)
		{
			m_local_matrix = mat4::CreateRotationFromQuaternion(m_local_rotation);
			m_local_matrix *= mat4::CreateTranslation(m_local_position);
			m_update_local_matrix = false;

			UpdateWorldMatrix();
		}
	}

	void SpotLightComponent::UpdateOrientedBox()
	{
		m_oriented_box->Transform(
			BOrientedBox(Vec3(0.0f, 0.0f, 0.5f), Vec3(sinf(m_cone_outer_angle), sinf(m_cone_outer_angle), 0.5f)),
			GetWorldMatrix());
	}

	void SpotLightComponent::SetAttenuationRadius(float radius)
	{
		m_attenuation_radius = radius;
		m_update_local_matrix = true;
	}

	void SpotLightComponent::SetConeInnerAngle(float degrees)
	{
		m_cone_inner_angle = math::ToRadian(degrees);
		m_cone_outer_angle = max(m_cone_inner_angle, m_cone_outer_angle);
		m_update_local_matrix = true;
	}

	void SpotLightComponent::SetConeOuterAngle(float degrees)
	{
		m_cone_outer_angle = math::ToRadian(degrees);
		m_cone_inner_angle = min(m_cone_inner_angle, m_cone_outer_angle);
		m_update_local_matrix = true;
	}
}
