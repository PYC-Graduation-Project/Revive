#pragma once
#include <client/object/level/core/level.h>

namespace revive
{
	using namespace client_fw;

	class LobbyLevel final : public Level
	{
	public:
		LobbyLevel();
		virtual ~LobbyLevel() = default;


		virtual bool Initialize() override;
		virtual void Shutdown() override;

		virtual void Update(float delta_time) override;
		virtual void ExecuteMessageFromServer(const SPtr<MessageEventInfo>& message) override;
	private:
		bool m_is_succeed_login = false;
	};
}