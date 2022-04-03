#include <include/client_core.h>
#include "object/actor/RevivePlayer.h"
#include "object/statemachine/state/player_state.h"

namespace revive
{
	void IdleState::Enter()
	{
		LOG_INFO("Idle State");
		m_player->SetAnimation("idle", true);
		m_player->SetMeshPosition(Vec3{ 0,40,0 });
	}

	SPtr<PlayerState> IdleState::ChageState()
	{
		float velocity = m_player->GetVelocity();
		int hp = m_player->GetHP();
		bool is_attacking = m_player->GetIsAttacking();
		bool is_hitting = m_player->GetIsHitting();

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
		LOG_INFO("Move State");
		m_player->SetAnimation("run", true);
	}

	SPtr<PlayerState> MoveState::ChageState()
	{
		float velocity = m_player->GetVelocity();
		int hp = m_player->GetHP();
		bool is_attacking = m_player->GetIsAttacking();
		bool is_hitting = m_player->GetIsHitting();

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
		LOG_INFO("Dead State");
		m_player->SetMeshPosition(Vec3{ 0,0,0 });
		m_player->SetAnimation("death", false);
		//m_player->SetActorState(eActorState::kDead);
	}


	void AttackState::Enter()
	{
		LOG_INFO("Attack State");
		m_player->SetMeshPosition(Vec3{ 0,0,0 });
		m_player->SetAnimation("attack", false);
	}

	SPtr<PlayerState> AttackState::ChageState()
	{
		int hp = m_player->GetHP();
		bool is_attacking = m_player->GetIsAttacking();
		bool is_hitting = m_player->GetIsHitting();

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
		LOG_INFO("HitState");
		m_player->SetMeshPosition(Vec3{ 0,0,0 });
		m_player->SetAnimation("hit", false);
		m_player->SetAnimationSpeed(0.6f);
	}

	SPtr<PlayerState> HitState::ChageState()
	{
		int hp = m_player->GetHP();
		int hit_count = m_player->GetHitCount();
		bool is_hitting = m_player->GetIsHitting();

		if (hp == 0)
			return CreateSPtr<DeadState>();
		if (hit_count > 0)// 맞는 중 또 맞을 수 있다. 이때는 공격 불가
		{
			m_player->DecrementHitCount();
			Enter();
		}
		if (is_hitting == false)
			return CreateSPtr<IdleState>();

		return nullptr;
	}

	void HitState::Exit()
	{
		m_player->SetAnimationSpeed(1.0f);
	}

}