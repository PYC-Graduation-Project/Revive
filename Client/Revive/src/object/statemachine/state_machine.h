#pragma once

namespace revive
{
	using namespace client_fw;

	class RevivePlayer;
	class DefaultPlayer;
	class PlayerState;

	class PlayerFSM
	{
	public:
		PlayerFSM() = default;
		
		void Initialize(const SPtr<RevivePlayer>& player);
		void Initialize(const SPtr<DefaultPlayer>& player);
		void Update();
		void Shutdown();

	private:
		WPtr<DefaultPlayer> m_other_player;
		WPtr<RevivePlayer> m_player;
		SPtr<PlayerState> m_curr_state;
	};
}

