#pragma once

namespace revive
{
	using namespace client_fw;

	class RevivePlayer;
	class PlayerState;

	class PlayerFSM
	{
	public:
		PlayerFSM() = default;
		
		void Initialize(const SPtr<RevivePlayer>& player);
		void Update();
		void Shutdown();

	private:
		WPtr<RevivePlayer> m_player;
		SPtr<PlayerState> m_curr_state;
	};
}

