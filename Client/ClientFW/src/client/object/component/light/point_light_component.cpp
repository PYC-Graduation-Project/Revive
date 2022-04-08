#include "stdafx.h"
#include "client/object/component/light/point_light_component.h"
#include "client/object/actor/core/actor.h"

namespace client_fw
{
	PointLightComponent::PointLightComponent(const std::string& name,
		const std::string& draw_shader_name)
		: LocalLightComponent(eLightType::kPoint, name, draw_shader_name)
	{
	}

	void PointLightComponent::UpdateWorldMatrix()
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
			m_world_matrix *= mat4::CreateTranslation(m_world_position);

			UpdateOrientedBox();

			m_is_updated_world_matrix = true;
		}
	}

	void PointLightComponent::UpdateLocalMatrix()
	{
		if (m_update_local_matrix)
		{
			m_local_matrix = mat4::CreateTranslation(m_local_position);
			m_update_local_matrix = false;

			UpdateWorldMatrix();
		}
	}

	void PointLightComponent::UpdateOrientedBox()
	{
		m_oriented_box->Transform(BOrientedBox(), GetWorldMatrix());
		float extent = m_attenuation_radius;
		m_oriented_box->SetExtents(Vec3(extent, extent, extent));
	}
}
