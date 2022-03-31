#include "object/statemachine/state/core/state.h"

namespace revive
{
	class IdleState : public PlayerState
	{
	public:
		IdleState() = default;
		virtual void Enter() override;
		virtual SPtr<PlayerState> ChageState()override;
	};

	class MoveState : public PlayerState
	{
	public:
		MoveState() = default;
		virtual void Enter() override;
		virtual SPtr<PlayerState> ChageState()override;
	};

	class DeadState : public PlayerState
	{
	public:
		DeadState() = default;
		virtual void Enter() override;
	};

	class AttackState : public PlayerState
	{
	public:
		AttackState() = default;
		virtual void Enter() override;
		virtual SPtr<PlayerState> ChageState()override;
	};

	class HitState : public PlayerState
	{
	public:
		HitState() = default;
		virtual void Enter() override;
		virtual SPtr<PlayerState> ChageState()override;
		virtual void Exit() override;
	};
}