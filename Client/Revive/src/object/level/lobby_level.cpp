#include <include/client_core.h>
#include <client/input/input.h>
#include <client/object/level/core/level_manager.h>
#include <client/object/level/core/level_loader.h>
#include <client/event/packetevent/packet_helper.h>
#include <client/event/messageevent/message_helper.h>
#include "revive_server/message/message_event_info.h"
#include "object/level/game_play_level.h"
#include "object/level/lobby_level.h"

//std::string g_id;
//std::string g_pw;

namespace revive
{
	LobbyLevel::LobbyLevel()
		:Level("lobby level")
	{
		LOG_INFO("�κ񷹺� ����");
	}
	bool LobbyLevel::Initialize()
	{
		LOG_INFO("�κ񷹺� �ʱ�ȭ");

		std::cin >> m_id;
		std::cin >> m_pw;
		std::cout << "id:" << m_id << "pw:" << m_pw << std::endl;

		//ȸ�� ���� 5, �α��� 6, ��Ī 7
		RegisterPressedEvent("send sign up", { { eKey::k5 } },
			[this]()->bool {
			
			PacketHelper::RegisterPacketEventToServer(CreateSPtr<SignUpMessageEventInfo>(HashCode("send sign up"), m_id.data(), m_pw.data()));
			return true;
		});
		RegisterPressedEvent("send sign in", { { eKey::k6 } },
			[this]()->bool {

		PacketHelper::RegisterPacketEventToServer(CreateSPtr<SignInMessageEventInfo>(HashCode("send sign in"), m_id.data(), m_pw.data()));
			return true;
		});
		RegisterPressedEvent("send sign matching", { { eKey::k7 } },
			[this]()->bool {
			if (m_is_succeed_login)
			{
				LOG_INFO("��Ī �����...");
				PacketHelper::RegisterPacketEventToServer(CreateSPtr<MatchingMessageEventInfo>(HashCode("send sign matching"), 2));
			}
				return true;
		});
		/*Input::SetInputMode(eInputMode::kGameOnly);
		Input::SetHideCursor(false);*/

		return true;
	}
	void LobbyLevel::Shutdown()
	{
		LOG_INFO("�κ� ���� �̴ٿ�");

		/*Input::SetHideCursor(false);
		Input::SetClipCursor(false);
		Input::SetInputMode(eInputMode::kUIOnly);*/
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
		default:
			break;
		}
	}

}