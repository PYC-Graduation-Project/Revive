#pragma once
#include <client/object/actor/core/actor.h>

namespace client_fw
{
	class SimpleMovementComponent;
	class CharacterMovementComponent;
	class SkeletalMeshComponent;
	class SphereComponent;
	class BoxComponent;
	class WidgetComponent;
}

namespace revive
{
	using namespace client_fw;

	class Weapon;
	class EnemyInfoUILayer;

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
		float m_hp = 1;
		float m_max_hp = 1;

		float m_disappear_time = 0.0f;

		int m_network_id = 9999;

		float m_time = 0.0f;


		bool m_is_disappearing = false;
		bool m_is_dead = false;
		bool m_is_attacking = false;

		Vec3 mesh_rotate = vec3::ZERO;
		Vec3 m_target_position = vec3::ZERO;

		Vec3 m_velocity;
		Vec3 m_inter_velocity;
		Vec3 m_next_pos;
		Vec3 m_previous_pos;

		SPtr<SimpleMovementComponent> m_simple_movement_component;
		SPtr<SkeletalMeshComponent> m_skeletal_mesh_component;
		SPtr<SphereComponent> m_blocking_sphere; //½Ì±Û
		SPtr<BoxComponent> m_blocking_box; //¸ÖÆ¼

		SPtr<SphereComponent> m_agro_sphere; 
		SPtr<BoxComponent> m_hit_box;
		SPtr<Weapon> m_weapon;

		void FixYPosition();
		Quaternion FindLookAtRotation(const Vec3& start, const Vec3& target);
		virtual void Attack();
		virtual bool SetCollisionComponent() { return true; }

	private:
		std::string m_mesh_path;
	
	public:
		const float GetHP() const { return m_hp; }
		void SetHP(const float hp) { m_hp = hp; }
		float GetMaxHP() const { return m_max_hp; }
		void SetMaxHP(float max_hp) { m_max_hp = max_hp; }

		void Hit(int damage = 1, int nw_id = 9999);

		void SetNetworkPosition(const Vec3& pos);

		const SPtr<BoxComponent>& GetHitBox() { return m_hit_box; }

	protected:
		SPtr<WidgetComponent> m_widget_component;
		SPtr<EnemyInfoUILayer> m_ui_layer;
		std::function<void(int id)> m_attack_packet_state_function;
		std::function<void()> m_mob_count_decrement_function;

	public:
		void OnAttackPacketStateFunction(std::function<void(int id)> function) { m_attack_packet_state_function = function; }
		void OnMobCountDecrementFunction(std::function<void()> function) { m_mob_count_decrement_function = function; }
	};
}
