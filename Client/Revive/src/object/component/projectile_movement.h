#pragma once
#include <client/object/component/util/core/movement_component.h>

namespace revive
{
	using namespace client_fw;

	class ProjectileMovementComponent : public MovementComponent
	{
	public:
		ProjectileMovementComponent(const std::string& name = "projectile movement component");
		virtual ~ProjectileMovementComponent() = default;

		virtual bool Initialize() override;
		virtual void Update(float delta_time) override;

	protected:
		//투사체의 초기 속력, 이 값이 0이 아니면 veolocity에 덮어씌워진다.
		float m_initial_speed = 0.0f;

		//투사체 중력 크기을 조정한다, 0이면 중력 없이 일직선으로 나간다.
		float m_projectile_gravity_scale;

		float m_max_speed = 0.0f;
		Vec3 m_acceleration = vec3::ZERO;

		//투사체 속도 벡터, 기본 
		Vec3 m_velocity = Vec3{0.0f,0.0f,1.0f};
		Vec3 m_initial_velocity;

	public:
		const Vec3& GetVelocity() const { return m_velocity; }

		void SetMaxSpeed(const float max_speed) { m_max_speed = max_speed; }

		void SetInitialSpeed(const float initial_speed) { m_initial_speed = initial_speed; }

		void SetProjectileGravityScale(const float projectile_gravity_scale) { m_projectile_gravity_scale = projectile_gravity_scale; }
	};
}