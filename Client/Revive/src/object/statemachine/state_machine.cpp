#include <include/client_core.h>
#include "object/statemachine/state_machine.h"
#include "object/statemachine/state/player_state.h"
#include "state_machine.h"
namespace revive
{
	void PlayerFSM::Initialize(const SPtr<RevivePlayer>& player)
	{
		m_player = player;
		m_curr_state = CreateSPtr<IdleState>();
		m_curr_state->Initialize(m_player);
	}
	void PlayerFSM::Update()
	{
		SPtr<PlayerState> state = m_curr_state->ChageState();
		if (state != nullptr)
		{
			//LOG_INFO("Change State");
			m_curr_state->Exit();
			m_curr_state = state;
			m_curr_state->Initialize(m_player);
		}
		m_curr_state->Update();
	}
	void PlayerFSM::Shutdown()
	{
		m_curr_state = nullptr;
	}
}