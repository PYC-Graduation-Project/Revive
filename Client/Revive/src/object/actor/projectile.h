#pragma once
#include <client/object/actor/core/actor.h>

namespace client_fw
{
	class SphereComponent;
}

namespace revive
{
	using namespace client_fw;

	class ProjectileMovementComponent;
	//LifeSpan ���� �ʿ� ������ ��(��) �� ������ �ش���ʹ� �Ҹ��Ѵ�.
	//�浹 ó�����ƴ϶� ������ ���� �ű⿡ �ݸ��������� �浹 ����?
	class Projectile : public Actor
	{
	public:
		Projectile(const std::string& name = "Projectile");
		virtual ~Projectile() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;
		virtual void Update(float delta_time) override;
	private:
		SPtr<SphereComponent> m_sphere_component; //?
		SPtr<StaticMeshComponent> m_static_mesh_component;
		SPtr<ProjectileMovementComponent> m_projectile_movement_component;

		//������ ���� 0.0f == ���� ����, 
		//�� ���� ��Ÿ�ӿ� �����Ǹ� �ȵȴ�.
		float m_life_span = 0.0f;

		//������ ����(�����������κ��� �帥 �ð�)
		float m_age = 0.0f;
	public:
		//����ü�� Velocity�� �����Ѵ�. ũ��� InitialSpeed���� ���� ��������.
		//���� ������ �����ϴµ� ���δٰ� �����.
		void SetVelocity(const Vec3& velocity);
		void SetCollisionInfo(bool is_collision, const std::string collision_type, const std::string collisionable_types, bool genrate_collision_event);
		void SetOnCollisionResponse(const std::function<void(const SPtr<SceneComponent>& comp,
			const SPtr<Actor>& other_actor, const SPtr<SceneComponent>& other_comp)>& function);
		void SetBlockingSphereRadius(float radius);
	};
}
