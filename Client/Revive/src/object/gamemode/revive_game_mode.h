#pragma once
#include <client/object/level/gamemode/game_mode_base.h>

namespace revive 
{
	using namespace client_fw;

	class RevivePlayer;

	class ReviveGameMode : public GameMode
	{
	public:
		ReviveGameMode(bool is_develop_mode);
		virtual ~ReviveGameMode() = default;

		virtual bool Initialize(const SPtr<Level>& level) override;
	};

}
