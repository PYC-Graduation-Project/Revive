#pragma once
#include <client/object/actor/pawn.h>
#include <client/object/actor/default_pawn.h>

namespace client_fw
{
	class SceneComponent;
	class SkeletalMeshComponent;
	class BoxComponent;
	class SphereComponent;
	class RenderCameraComponent;
	class SimpleMovementComponent;
	class CharacterMovementComponent;
}

namespace revive
{
	using namespace client_fw;

	class Pistol;
	class PlayerFSM;

	class DefaultPlayer : public Pawn
	{
	public:
		DefaultPlayer(const std::string& name = "player");
		virtual ~DefaultPlayer() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;
		virtual void Update(float delta_time) override;
		virtual void ExecuteMessageFromServer(const SPtr<MessageEventInfo>& message) override;

	protected:
		int m_hp = 10;
		int m_hit_count = 0;//�´� ���� �� �´� ��츦 ���� ����
		int m_network_id = 9999;
		float m_time = 0.0f;
		float m_stop_time = 0.0f;
		float m_speed = 0.f;

		bool m_is_attacking = false;
		bool m_is_hitting = false;
		bool m_is_dying = false;
		bool m_is_fire = false;

		Vec3 m_velocity;
		Vec3 m_previous_velocity = vec3::ZERO;
		Vec3 m_inter_velocity;
		Vec3 m_next_pos;
		Vec3 m_previous_pos;

		std::string m_mesh_path;
		SPtr<PlayerFSM> m_player_fsm;
		SPtr<CharacterMovementComponent> m_character_movement_component;
		SPtr<SkeletalMeshComponent> m_skeletal_mesh_component;
		SPtr<BoxComponent> m_hit_box;
		std::array<SPtr<Pistol>, 2> m_weapon;

		virtual Quaternion FindLookAtRotation(const Vec3& start, const Vec3& target);
		void PlayerInterpolation(float delta_time);

		SPtr<DefaultPlayer> SharedFromThis() { return std::static_pointer_cast<DefaultPlayer>(shared_from_this()); }

	public:
		virtual const float GetVelocity() const;
		const int GetHitCount() const { return m_hit_count; }
		const int GetHP() const { return m_hp; }
		const bool GetIsAttacking() const { return m_is_attacking; }
		const bool GetIsHitting() const { return m_is_hitting; }
		const bool GetIsDying() const { return m_is_dying; }
		const int GetNetworkID()const { return m_network_id; }
		void SetIsDying(bool value) { m_is_dying = value; }
		void SetAnimation(const std::string& animation_name, bool looping);
		void SetMeshPosition(const Vec3& pos);
		void SetAnimationSpeed(float speed);
		void SetNetworkID(int val) { m_network_id = val; };
		void DecrementHitCount() { m_hit_count--; }

		virtual void Attack();
		virtual void Hit(int damage = 1);
	};

	class RevivePlayer : public DefaultPlayer
	{
	public:
		RevivePlayer(const std::string& name = "Player");
		virtual ~RevivePlayer() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;
		virtual void Update(float delta_time) override;
	private:

		bool m_is_cheating = false;

		void AddMovementInput(const Vec3& direction, float scale) override;
		
		void RegisterEvent(); //����� ���� �������� ���� �и��ߴ�.

		//player�� state�� �����ϴ� ��ü
		//������ State��ü�� �����ϰ� �÷��̾� Update���� �ٲ��ַ��� ������,
		//�̷��� �� ��� ĳ������ �Ź� �Ͼ�� ������ PlayerFSM���� ������ ���ִ� ���·� ������

		SPtr<CharacterMovementComponent> m_movement_component;
		SPtr<SphereComponent> m_blocking_sphere;
		SPtr<RenderCameraComponent> m_camera_component;

		using Pawn::SetUseControllerPitch;
		using Pawn::SetUseControllerYaw;
		using Pawn::SetUseControllerRoll;

		SPtr<RevivePlayer> SharedFromThis() { return std::static_pointer_cast<RevivePlayer>(shared_from_this()); }

	public:
		void CollisionResponse(const SPtr<SceneComponent>& component, const SPtr<Actor>& other_actor, const SPtr<SceneComponent>& other_component) {};

		const SPtr<RenderCameraComponent>& GetCameraComponent() { return m_camera_component; }
		//void SetIsAttacking(bool value) { m_is_attacking = value; }
		//void SetIsHitting(bool value) { m_is_hitting = value; }
		const float GetVelocity() const;
		const int GetHitCount() const { return m_hit_count; }
		const int GetHP() const { return m_hp; }


		virtual void Attack() override;
		virtual void Hit(int damage) override;


		void DecrementHP() { m_hp--; LOG_INFO("my HP : {0}", m_hp); }

		//void RotateFromDirection(const Vec3& direction);
		//Controller���� ȣ��
		//void MinPitch(); //�ּ� Pitch ������ �ɱ� ���� �Լ�
		void FixYPosition();

	};

	class DefaultCharacter : public DefaultPawn
	{
	public:
		DefaultCharacter(const std::string& name = "default character");
		virtual ~DefaultCharacter() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;
		virtual void Update(float delta_time) override;

		void CollisionResponse(const SPtr<SceneComponent>& component, const SPtr<Actor>& other_actor, const SPtr<SceneComponent>& other_component);

	private:
		using Pawn::SetUseControllerPitch;
		using Pawn::SetUseControllerYaw;
		using Pawn::SetUseControllerRoll;

	};
}