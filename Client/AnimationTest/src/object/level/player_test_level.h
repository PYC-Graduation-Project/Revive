#pragma once
#include "client/object/level/core/level.h"

namespace animation_test
{
	using namespace client_fw;

	class PlayerTestLevel final : public Level
	{
	public:
		PlayerTestLevel();
		virtual ~PlayerTestLevel() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;

		virtual void Update(float delta_time) override;

	private:
		SPtr<class DefaultPawn> m_player;
		SPtr<class PlayerController> m_player_controller;
	};

}

