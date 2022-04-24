#include <include/client_core.h>
#include <client/input/input.h>
#include <client/object/level/core/level_manager.h>
#include <client/object/level/core/level_loader.h>
#include <client/event/packetevent/packet_helper.h>
#include <client/event/messageevent/message_helper.h>
#include "revive_server/message/message_event_info.h"
#include "object/level/game_play_level.h"
#include "object/level/lobby_level.h"

std::string g_id;
std::string g_pw;

namespace revive
{
	LobbyLevel::LobbyLevel()
		:Level("lobby level")
	{
	}
	bool LobbyLevel::Initialize()
	{
		std::cin >> g_id;
		std::cin >> g_pw;
		std::cout << "id:" << g_id << "pw:" << g_pw << std::endl;

		//회원 가입 5, 로그인 6, 매칭 7
		RegisterPressedEvent("send sign up", { { eKey::k5 } },
			[this]()->bool {
			
			PacketHelper::RegisterPacketEventToServer(CreateSPtr<SignUpMessageEventInfo>(HashCode("send sign up"), g_id.data(), g_pw.data()));
			return true;
		});
		RegisterPressedEvent("send sign in", { { eKey::k6 } },
			[this]()->bool {

			PacketHelper::RegisterPacketEventToServer(CreateSPtr<SignInMessageEventInfo>(HashCode("send sign in"), g_id.data(), g_pw.data()));
			return true;
		});
		RegisterPressedEvent("send sign matching", { { eKey::k7 } },
			[this]()->bool {
			if (m_is_succeed_login)
			{
				LOG_INFO("매칭 대기중...");
				Input::SetHideCursor(false);
				Input::SetInputMode(eInputMode::kUIOnly);
				PacketHelper::RegisterPacketEventToServer(CreateSPtr<MatchingMessageEventInfo>(HashCode("send sign matching"), 2));
				return true;
			}
		});

		Input::SetInputMode(eInputMode::kGameOnly);
		Input::SetHideCursor(false);

		return true;
	}
	void LobbyLevel::Shutdown()
	{
		Input::SetHideCursor(false);
		Input::SetClipCursor(false);
		Input::SetInputMode(eInputMode::kUIOnly);
	}
	void LobbyLevel::Update(float delta_time)
	{
	}
	void LobbyLevel::ExecuteMessageFromServer(const SPtr<MessageEventInfo>& message)
	{
		switch (message->GetEventID())
		{
		case HashCode("match"):
		{
			auto msg = std::static_pointer_cast<MatchingMessageOKEventInfo>(message);
			LevelManager::GetLevelManager().OpenLevel(CreateSPtr<GamePlayLevel>(), nullptr);
			break;
		}
		case HashCode("sign in"):
		{
			auto msg = std::static_pointer_cast<SignInMessageOkEventInfo>(message);
			m_is_succeed_login = true;
			break;
		}
		}
	}
}