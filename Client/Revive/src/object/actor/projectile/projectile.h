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
	//LifeSpan 구현 필요 정해진 값(초) 가 지나면 해당액터는 소멸한다.

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

		//액터의 수명 0.0f == 죽지 않음, 
		//이 값은 런타임에 수정되면 안된다.
		float m_life_span = 0.0f;

		//액터의 나이(생성시점으로부터 흐른 시간)
		float m_age = 0.0f;

		void SetGravityScale(float gravity_scale);
		void SetInitialSpeed(float initial_speed);
	public:
		//투사체의 Velocity를 설정한다. 크기는 InitialSpeed값에 덮어 씌워진다.
		//따라서 방향을 결정하는데 쓰인다고 보면됨.
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
