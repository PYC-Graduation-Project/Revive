#pragma once
#include <client/object/actor/player_controller.h>

namespace revive
{
	using namespace client_fw;

	class ReviveController :public PlayerController
	{
	public:
		ReviveController();
		virtual ~ReviveController() = default;

		virtual bool Initialize() override;
		//virtual void Shutdown() override;

	private:
		void MinPItch();
	};
}
