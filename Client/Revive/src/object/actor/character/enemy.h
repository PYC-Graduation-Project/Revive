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
		virtual void ExecuteMessageFromServer(const SPtr<MessageEventInfo>& message) override;
		const int GetNetworkID()const { return m_network_id; }
		void SetNetworkID(int val) { m_network_id = val; };
	protected:
		int m_hp;
		float m_disappear_time = 0.0f;
		int m_network_id = 9999;
		bool m_is_disappearing = false;
		bool m_is_dead = false;
		bool m_is_attacking = false;

		Vec3 mesh_rotate = vec3::ZERO;
		Vec3 m_target_position = vec3::ZERO;

		SPtr<SkeletalMeshComponent> m_skeletal_mesh_component;
		SPtr<SphereComponent> m_blocking_sphere; //�̱�
		SPtr<BoxComponent> m_blocking_box; //��Ƽ

		SPtr<SphereComponent> m_agro_sphere; 
		SPtr<BoxComponent> m_hit_box;

		void FixYPosition();
		Quaternion FindLookAtRotation(const Vec3& start, const Vec3& target);
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
