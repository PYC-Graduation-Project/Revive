#pragma once
#include <client/object/actor/core/actor.h>

namespace client_fw
{
	class SceneComponent;
	class StaticMeshComponent;
	class SphereComponent;
}

namespace revive
{
	using namespace client_fw;

	class ProjectileMovementComponent;
	//LifeSpan ���� �ʿ� ������ ��(��) �� ������ �ش���ʹ� �Ҹ��Ѵ�.

	class Projectile : public Actor
	{
	public:
		Projectile(const std::string& name = "Projectile");
		virtual ~Projectile() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;
		virtual void Update(float delta_time) override;

	protected:
		SPtr<SphereComponent> m_sphere_component; 
		SPtr<StaticMeshComponent> m_static_mesh_component;
		SPtr<ProjectileMovementComponent> m_projectile_movement_component;

		bool m_is_collision = true;

		//������ ���� 0.0f == ���� ����, 
		//�� ���� ��Ÿ�ӿ� �����Ǹ� �ȵȴ�.
		float m_life_span = 0.0f;

		//������ ����(�����������κ��� �帥 �ð�)
		float m_age = 0.0f;

		void SetGravityScale(float gravity_scale);
		void SetInitialSpeed(float initial_speed);
	public:
		//����ü�� Velocity�� �����Ѵ�. ũ��� InitialSpeed���� ���� ��������.
		//���� ������ �����ϴµ� ���δٰ� �����.
		void SetIsCollision(const bool value) { m_is_collision = value; }
		void SetVelocity(const Vec3& velocity);
		void SetCollisionInfo(bool is_collision, std::string&& collision_type, std::set<std::string>&& collisionable_types, bool genrate_collision_event);
		void SetOnCollisionResponse(const std::function<void(const SPtr<SceneComponent>& comp,
			const SPtr<Actor>& other_actor, const SPtr<SceneComponent>& other_comp)>& function);
		void SetBlockingSphereRadius(float radius);

		const float GetSphereRadius() const;
		const bool GetIsCollision() const { return m_is_collision; }
	};
}
