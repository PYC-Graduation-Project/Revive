#include <include/client_core.h>
#include "object/actor/character/revive_player.h"
#include "object/statemachine/state/player_state.h"

namespace revive
{
	void IdleState::Enter()
	{
		const auto& player = m_player.lock();
		
		//LOG_INFO(player->GetName() + "Idle State");
		player->SetAnimation("idle", true);
	}

	SPtr<PlayerState> IdleState::ChageState()
	{
		const auto& player = m_player.lock();
		float velocity = player->GetVelocity();
		float hp = player->GetHP();
		bool is_attacking = player->IsAttacking();
		bool is_hitting = player->IsHitting();

		if (hp == 0)
			return CreateSPtr<DeadState>();
		if (is_hitting == true)// 가만히 있는데 맞을 수 있다.
			return CreateSPtr<HitState>();
		if (is_attacking)
			return CreateSPtr<AttackState>();
		if (velocity > 0.0f)
			return CreateSPtr<MoveState>();
		return nullptr;
	}

	void MoveState::Enter()
	{
		const auto& player = m_player.lock();
		//LOG_INFO(player->GetName() + "Move State");
		player->SetAnimation("run", true);
	}

	SPtr<PlayerState> MoveState::ChageState()
	{
		const auto& player = m_player.lock();
		float velocity = player->GetVelocity();
		float hp = player->GetHP();
		bool is_attacking = player->IsAttacking();
		bool is_hitting = player->IsHitting();

		if (hp == 0)
			return CreateSPtr<DeadState>();
		if (is_hitting == true)// 이동중 맞을 수 있다.
			return CreateSPtr<HitState>();
		if (is_attacking)
			return CreateSPtr<AttackState>();
		if (math::NearZero(velocity))
			return CreateSPtr<IdleState>();
		return nullptr;
	}

	void DeadState::Enter()
	{
		//LOG_INFO("Dead State");
		const auto& player = m_player.lock();
		player->SetIsDying(true);
		player->SetAnimation("death", false);
	}


	void AttackState::Enter()
	{
		const auto& player = m_player.lock();
		//LOG_INFO(player->GetName() + "Attack State");
		player->SetAnimation("attack_first", false);
		player->SetAnimationSpeed(1.1f);
		//player->Attack();
	}

	SPtr<PlayerState> AttackState::ChageState()
	{
		const auto& player = m_player.lock();
		float hp = player->GetHP();
		bool is_attacking = player->IsAttacking();
		bool is_hitting = player->IsHitting();

		if (hp == 0)
			return CreateSPtr<DeadState>();
		if (is_hitting == true)// 공격중 맞을 수 있다.
			return CreateSPtr<HitState>();
		if (is_attacking == false)
			return CreateSPtr<IdleState>();
		return nullptr;
	}

	void HitState::Enter()
	{
		//LOG_INFO("HitState");
		const auto& player = m_player.lock();
		player->SetMeshPosition(Vec3{ 0,10,0 });
		player->SetAnimation("hit", false);
		player->SetAnimationSpeed(0.6f);
	}

	SPtr<PlayerState> HitState::ChageState()
	{
		const auto& player = m_player.lock();
		float hp = player->GetHP();
		int hit_count = player->GetHitCount();
		bool is_hitting = player->IsHitting();

		if (hp == 0)
			return CreateSPtr<DeadState>();
		if (hit_count > 0)// 맞는 중 또 맞을 수 있다. 이때는 공격 불가
		{
			player->DecrementHitCount();
			Enter();
		}
		if (is_hitting == false)
			return CreateSPtr<IdleState>();

		return nullptr;
	}

	void HitState::Exit()
	{
		const auto& player = m_player.lock();
		player->SetMeshPosition(Vec3{ 0,0,0 });
		player->SetAnimationSpeed(1.0f);
	}

}