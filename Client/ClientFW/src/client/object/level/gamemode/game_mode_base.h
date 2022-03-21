#pragma once

namespace client_fw
{
	class PlayerController;
	class Pawn;
	class Level;

	class GameMode
	{
	public:
		GameMode();
		virtual ~GameMode();

		virtual bool Initialize(const SPtr<Level>& level);
		virtual void Shutdown();

	protected:
		SPtr<PlayerController> m_player_controller;
		SPtr<Pawn> m_default_pawn;

	public:
		const SPtr<PlayerController>& GetPlayerController() const { return m_player_controller; }
		const SPtr<Pawn>& GetDefaultPawn() const { return m_default_pawn; }
	};
}


