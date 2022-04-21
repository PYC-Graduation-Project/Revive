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

	protected:
		int m_hp = 10;
		int m_hit_count = 0;//�´� ���� �� �´� ��츦 ���� ����

		bool m_is_attacking = false;
		bool m_is_hitting = false;
		bool m_is_dying = false;
		

		std::string m_mesh_path;
		SPtr<PlayerFSM> m_player_fsm;
		SPtr<CharacterMovementComponent> m_movement_component;
		SPtr<SkeletalMeshComponent> m_skeletal_mesh_component;
		SPtr<BoxComponent> m_hit_box;
		std::array<SPtr<Pistol>, 2> m_weapon;

		void RotatePlayerFromCameraDirection(Vec3& dest_direction);

		SPtr<DefaultPlayer> SharedFromThis() { return std::static_pointer_cast<DefaultPlayer>(shared_from_this()); }

	public:
		const int GetHitCount() const { return m_hit_count; }
		const float GetVelocity() const;
		const int GetHP() const { return m_hp; }
		const bool GetIsAttacking() const { return m_is_attacking; }
		const bool GetIsHitting() const { return m_is_hitting; }
		const bool GetIsDying() const { return m_is_dying; }

		void SetIsDying(bool value) { m_is_dying = value; }
		void SetAnimation(const std::string& animation_name, bool looping);
		void SetMeshPosition(const Vec3& pos);
		void SetAnimationSpeed(float speed);

		void DecrementHitCount() { m_hit_count--; }

		void Attack();
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

		virtual void AddMovementInput(const Vec3& direction, float scale) override;
		virtual void Hit(int damage) override;
		void RegisterEvent(); //����� ���� �������� ���� �и��ߴ�.
		void MinPitch(); //�ּ� Pitch ������ �ɱ� ���� �Լ�
		void FixYPosition();

		//player�� state�� �����ϴ� ��ü
		//������ State��ü�� �����ϰ� �÷��̾� Update���� �ٲ��ַ��� ������,
		//�̷��� �� ��� ĳ������ �Ź� �Ͼ�� ������ PlayerFSM���� ������ ���ִ� ���·� ������

		SPtr<SphereComponent> m_blocking_sphere;
		SPtr<RenderCameraComponent> m_camera_component;

		using Pawn::SetUseControllerPitch;
		using Pawn::SetUseControllerYaw;
		using Pawn::SetUseControllerRoll;

	public:
		void CollisionResponse(const SPtr<SceneComponent>& component, const SPtr<Actor>& other_actor, const SPtr<SceneComponent>& other_component) {};

		const SPtr<RenderCameraComponent>& GetCameraComponent() { return m_camera_component; }
		//void SetIsAttacking(bool value) { m_is_attacking = value; }
		//void SetIsHitting(bool value) { m_is_hitting = value; }
		void DecrementHP() { m_hp--; LOG_INFO("my HP : {0}", m_hp); }

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