#include "stdafx.h"
#include "client/object/component/util/simple_movement_component.h"

namespace client_fw
{
	SimpleMovementComponent::SimpleMovementComponent(const std::string& name)
		: MovementComponent(name)
	{
	}

	void SimpleMovementComponent::Update(float delta_time)
	{
		if (m_move_input != vec3::ZERO)
		{
			m_move_input.Normalize();
			m_cur_speed += m_acceleration * delta_time;
		}
		else
		{
			m_cur_speed -= m_deceleration * delta_time;
		}
		m_cur_speed = std::clamp(m_cur_speed, 0.0f, m_max_speed);
		m_velocity = m_move_input * m_cur_speed;

		MovementComponent::Update(delta_time);
	}
}
