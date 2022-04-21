#pragma once
#include "object/actor/character/revive_player.h"

namespace revive
{
	using namespace client_fw;

	class PlayerState 
	{
	public:
		PlayerState() = default;

		virtual void Initialize(const WPtr<DefaultPlayer>& player) { m_player = player; Enter(); }

		virtual void Enter() {} //State 진입 시 해줄 것
		virtual void Update() {} 
		virtual void Exit() {} //State 퇴장시 해줄 것

		virtual SPtr<PlayerState> ChageState() { return nullptr; } //State를 전환한다. (Transition)

	protected:
		WPtr<DefaultPlayer> m_player;
	};

	
}
