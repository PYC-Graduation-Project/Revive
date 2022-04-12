#pragma once
#include "object/actor/revive_player.h"

namespace revive
{
	using namespace client_fw;

	class PlayerState 
	{
	public:
		PlayerState() = default;

		virtual void Initialize(const SPtr<RevivePlayer>& player) { m_player = player; Enter(); }

		virtual void Enter() {} //State ���� �� ���� ��
		virtual void Update() {} 
		virtual void Exit() {} //State ����� ���� ��

		virtual SPtr<PlayerState> ChageState() { return nullptr; } //State�� ��ȯ�Ѵ�. (Transition)

	protected:
		SPtr<RevivePlayer> m_player;
	};

	
}