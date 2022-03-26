#pragma once

namespace revive 
{
	using namespace client_fw;

	class RevivePlayer;

	class ReviveGameMode : public GameMode
	{
	public:
		ReviveGameMode();
		virtual ~ReviveGameMode();

		virtual bool Initialize(const SPtr<Level>& level);
		virtual void Shutdown();
	private:
		SPtr<RevivePlayer> m_player;
	};

}
