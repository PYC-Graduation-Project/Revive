#pragma once
#include <client/object/level/core/level.h>

namespace revive
{
	using namespace client_fw;

	enum class eGameResult
	{
		kWin,
		kLose
	};

	class GameEndLevel final : public Level
	{
	public:
		GameEndLevel(eGameResult game_result);
		virtual ~GameEndLevel() = default;


		virtual bool Initialize() override;
		virtual void Shutdown() override;

		virtual void Update(float delta_time) override;
		virtual void ExecuteMessageFromServer(const SPtr<MessageEventInfo>& message) override;
	private:
		eGameResult m_game_result;
	};
}