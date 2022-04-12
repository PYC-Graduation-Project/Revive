#include <include/client_core.h>
#include <client/object/actor/core/actor.h>
#include "object/component/projectile_movement.h"

namespace revive
{
	ProjectileMovementComponent::ProjectileMovementComponent(const std::string& name)
		: MovementComponent(name)
	{
	}
	bool ProjectileMovementComponent::Initialize()
	{
		if (m_initial_speed > 0.f)
		{
			m_velocity.Normalize();
			m_velocity = m_velocity * m_initial_speed;
		}

		m_initial_velocity = m_velocity;
		return true;
	}
	void ProjectileMovementComponent::Update(float delta_time)
	{
		//SimpleMovementComponent::Update 와 유사하나 좀 다르다

		//0.0f이면 가속도는 없다. 중력이 없으면 총알은 등속도 운동을함.
		m_acceleration.y += -10.0f/*중력가속도라고 친다*/ * m_projectile_gravity_scale; 
		m_velocity += (m_acceleration * delta_time);

		if(m_max_speed > 0.f) 
			m_velocity = std::clamp(m_velocity.Length(), 0.0f, m_max_speed) * vec3::Normalize(m_velocity);

		//PawnMovementComponent::Update와 같다.
		if (math::NearZero(m_velocity.Length()) == false)
		{
			const auto& owner = m_owner.lock();
			if (owner != nullptr)
			{
				Vec3 pos = owner->GetPosition();
				pos += m_velocity * delta_time;
				owner->SetPosition(pos);
			}
			
		}
	}
}