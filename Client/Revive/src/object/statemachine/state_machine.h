#pragma once

namespace revive
{
	using namespace client_fw;

	class DefaultPlayer;
	class PlayerState;

	class PlayerFSM
	{
	public:
		PlayerFSM() = default;
		
		void Initialize(const SPtr<DefaultPlayer>& player);
		void Update();
		void Shutdown();

	private:
		WPtr<DefaultPlayer> m_player;
		SPtr<PlayerState> m_curr_state;
	};
}

