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
		//����ü�� �ʱ� �ӷ�, �� ���� 0�� �ƴϸ� veolocity�� ���������.
		float m_initial_speed = 0.0f;

		//����ü �߷� ũ���� �����Ѵ�, 0�̸� �߷� ���� ���������� ������.
		float m_projectile_gravity_scale;

		float m_max_speed = 0.0f;
		Vec3 m_acceleration = vec3::ZERO;

		//����ü �ӵ� ����, �⺻ 
		Vec3 m_velocity = Vec3{0.0f,0.0f,1.0f};
		Vec3 m_initial_velocity;

	public:
		const Vec3& GetVelocity() const { return m_velocity; }

		void SetMaxSpeed(const float max_speed) { m_max_speed = max_speed; }

		void SetInitialSpeed(const float initial_speed) { m_initial_speed = initial_speed; }

		void SetProjectileGravityScale(const float projectile_gravity_scale) { m_projectile_gravity_scale = projectile_gravity_scale; }
	};
}