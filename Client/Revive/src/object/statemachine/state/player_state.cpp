#include <include/client_core.h>
#include "object/actor/character/revive_player.h"
#include "object/statemachine/state/player_state.h"

namespace revive
{
	void IdleState::Enter()
	{
		//LOG_INFO("Idle State");
		if (m_other_player.expired())
		{
			const auto& player = m_player.lock();
			player->SetAnimation("idle", true);
			player->SetMeshPosition(Vec3{ 0,40,0 });
		}
		else
		{
			const auto& player = m_other_player.lock();
			player->SetAnimation("idle", true);
			player->SetMeshPosition(Vec3{ 0,40,0 });
		}

		
	}

	SPtr<PlayerState> IdleState::ChageState()
	{
		if (m_other_player.expired())
		{
			const auto& player = m_player.lock();
			float velocity = player->GetVelocity();
			int hp = player->GetHP();
			bool is_attacking = player->GetIsAttacking();
			bool is_hitting = player->GetIsHitting();

			if (hp == 0)
				return CreateSPtr<DeadState>();
			if (is_hitting == true)// ������ �ִµ� ���� �� �ִ�.
				return CreateSPtr<HitState>();
			if (is_attacking)
				return CreateSPtr<AttackState>();
			if (velocity > 0.0f)
				return CreateSPtr<MoveState>();
			return nullptr;
		}
		else
		{
			const auto& player = m_other_player.lock();
			float velocity = player->GetVelocity();
			int hp = player->GetHP();
			bool is_attacking = player->GetIsAttacking();
			bool is_hitting = player->GetIsHitting();

			if (hp == 0)
				return CreateSPtr<DeadState>();
			if (is_hitting == true)// ������ �ִµ� ���� �� �ִ�.
				return CreateSPtr<HitState>();
			if (is_attacking)
				return CreateSPtr<AttackState>();
			if (velocity > 0.0f)
				return CreateSPtr<MoveState>();
			return nullptr;
		}
		
	}

	void MoveState::Enter()
	{
		//LOG_INFO("Move State");
		if (m_other_player.expired())
		{
			const auto& player = m_player.lock();
			player->SetAnimation("run", true);
		}
		else
		{
			const auto& player = m_other_player.lock();
			player->SetAnimation("run", true);
		}
	}

	SPtr<PlayerState> MoveState::ChageState()
	{
		if (m_other_player.expired())
		{
			const auto& player = m_player.lock();
			float velocity = player->GetVelocity();
			int hp = player->GetHP();
			bool is_attacking = player->GetIsAttacking();
			bool is_hitting = player->GetIsHitting();

			if (hp == 0)
				return CreateSPtr<DeadState>();
			if (is_hitting == true)// �̵��� ���� �� �ִ�.
				return CreateSPtr<HitState>();
			if (is_attacking)
				return CreateSPtr<AttackState>();
			if (math::NearZero(velocity))
				return CreateSPtr<IdleState>();
			return nullptr;
		}
		else
		{
			const auto& player = m_other_player.lock();
			float velocity = player->GetVelocity();
			int hp = player->GetHP();
			bool is_attacking = player->GetIsAttacking();
			bool is_hitting = player->GetIsHitting();

			if (hp == 0)
				return CreateSPtr<DeadState>();
			if (is_hitting == true)// �̵��� ���� �� �ִ�.
				return CreateSPtr<HitState>();
			if (is_attacking)
				return CreateSPtr<AttackState>();
			if (math::NearZero(velocity))
				return CreateSPtr<IdleState>();
			return nullptr;
		}
		
	}

	void DeadState::Enter()
	{
		//LOG_INFO("Dead State");
		
		if (m_other_player.expired())
		{
			const auto& player = m_player.lock();
			player->SetIsDying(true);
			player->SetMeshPosition(Vec3{ 0,0,0 });
			player->SetAnimation("death", false);
		}
		else
		{
			const auto& player = m_other_player.lock();
			player->SetIsDying(true);
			player->SetMeshPosition(Vec3{ 0,0,0 });
			player->SetAnimation("death", false);
		}
		//player->SetActorState(eActorState::kDead);
	}


	void AttackState::Enter()
	{
		//LOG_INFO("Attack State");
		if (m_other_player.expired())
		{
			const auto& player = m_player.lock();
			player->SetMeshPosition(Vec3{ 0,0,0 });
			player->SetAnimation("attack", false);
			player->Attack();
		}
		else
		{
			const auto& player = m_other_player.lock();
			player->SetMeshPosition(Vec3{ 0,0,0 });
			player->SetAnimation("attack", false);
			player->Attack();
		}
	}

	SPtr<PlayerState> AttackState::ChageState()
	{
		if (m_other_player.expired())
		{
			const auto& player = m_player.lock();
			int hp = player->GetHP();
			bool is_attacking = player->GetIsAttacking();
			bool is_hitting = player->GetIsHitting();

			if (hp == 0)
				return CreateSPtr<DeadState>();
			if (is_hitting == true)// ������ ���� �� �ִ�.
				return CreateSPtr<HitState>();
			if (is_attacking == false)
				return CreateSPtr<IdleState>();
			return nullptr;
		}
		else
		{
			const auto& player = m_other_player.lock();
			int hp = player->GetHP();
			bool is_attacking = player->GetIsAttacking();
			bool is_hitting = player->GetIsHitting();

			if (hp == 0)
				return CreateSPtr<DeadState>();
			if (is_hitting == true)// ������ ���� �� �ִ�.
				return CreateSPtr<HitState>();
			if (is_attacking == false)
				return CreateSPtr<IdleState>();
			return nullptr;

		}
	}

	void HitState::Enter()
	{
		//LOG_INFO("HitState");
		if (m_other_player.expired())
		{
			const auto& player = m_player.lock();
			player->SetMeshPosition(Vec3{ 0,10,0 });
			player->SetAnimation("hit", false);
			player->SetAnimationSpeed(0.6f);
		}
		else
		{
			const auto& player = m_other_player.lock();
			player->SetMeshPosition(Vec3{ 0,10,0 });
			player->SetAnimation("hit", false);
			player->SetAnimationSpeed(0.6f);
		}
	}

	SPtr<PlayerState> HitState::ChageState()
	{
		if (m_other_player.expired())
		{
			const auto& player = m_player.lock();
			int hp = player->GetHP();
			int hit_count = player->GetHitCount();
			bool is_hitting = player->GetIsHitting();

			if (hp == 0)
				return CreateSPtr<DeadState>();
			if (hit_count > 0)// �´� �� �� ���� �� �ִ�. �̶��� ���� �Ұ�
			{
				player->DecrementHitCount();
				Enter();
			}
			if (is_hitting == false)
				return CreateSPtr<IdleState>();

			return nullptr;
		}
		else
		{
			const auto& player = m_other_player.lock();
			int hp = player->GetHP();
			int hit_count = player->GetHitCount();
			bool is_hitting = player->GetIsHitting();

			if (hp == 0)
				return CreateSPtr<DeadState>();
			if (hit_count > 0)// �´� �� �� ���� �� �ִ�. �̶��� ���� �Ұ�
			{
				player->DecrementHitCount();
				Enter();
			}
			if (is_hitting == false)
				return CreateSPtr<IdleState>();

			return nullptr;
		}
	}

	void HitState::Exit()
	{
		if (m_other_player.expired())
		{
			const auto& player = m_player.lock();
			player->SetAnimationSpeed(1.0f);
		}
		else
		{
			const auto& player = m_other_player.lock();
			player->SetAnimationSpeed(1.0f);
		}
	}

}