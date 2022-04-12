#pragma once
#include <client/object/actor/core/actor.h>

namespace client_fw
{
	class SkeletalMeshComponent;
	class SphereComponent;
	class BoxComponent;
}

namespace revive
{
	using namespace client_fw;

	class Enemy : public Actor
	{
	public:
		Enemy(const std::string& mesh_path, const std::string& name = "Enemy");
		virtual ~Enemy() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;
		virtual void Update(float delta_time) override;

	protected:
		int m_hp;
		float m_disappear_time = 0.0f;
		bool m_is_disappearing = false;
		bool m_is_dead = false;
		bool m_is_attacking = false;

		SPtr<SkeletalMeshComponent> m_skeletal_mesh_component;
		SPtr<SphereComponent> m_blocking_sphere; //½Ì±Û
		SPtr<BoxComponent> m_blocking_box; //¸ÖÆ¼

		SPtr<SphereComponent> m_agro_sphere; 
		SPtr<SphereComponent> m_attack_sphere;
		std::vector<SPtr<BoxComponent>> m_hit_boxes;

		void FixYPosition();
		void RotateFromPlayer(const Vec3& direction);
		virtual void Attack();
		virtual bool SetCollisionComponent() { return true; }
	private:
		std::string m_mesh_path;
	
	public:
		const int GetHP() const { return m_hp; }
		void SetHP(const int hp) { m_hp = hp; }

		void Hit(int damage = 1);
	};
}
