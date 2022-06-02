#include <include/client_core.h>
#include <client/input/input.h>
#include <client/object/level/core/level_manager.h>
#include <client/object/level/core/level_loader.h>
#include <client/event/packetevent/packet_helper.h>
#include <client/event/messageevent/message_helper.h>
#include <client/asset/sound/core/sound_manager.h>
#include <client/asset/sound/sound.h>

#include "revive_server/message/message_event_info.h"
#include "object/level/game_play_level.h"
#include "object/level/lobby_level.h"
#include "object/ui/lobby_ui_layer.h"

#include "server/network_move_object.h"
#include"server/network.h"
//std::string g_id;
//std::string g_pw;

namespace revive
{
	LobbyLevel::LobbyLevel()
		:Level("lobby level")
	{
		client_fw::SoundManager::GetSoundManager().LoadSound("Contents/Sound/1a.ogg");
	}
	bool LobbyLevel::Initialize()
	{
		client_fw::SoundManager::GetSoundManager().GetSound(0)->Play(true);
		m_lobby_ui_layer = CreateSPtr<LobbyUILayer>();
		RegisterUILayer(m_lobby_ui_layer);

		Input::SetInputMode(eInputMode::kUIAndGame);
		Network::matching_end = false;
//#ifdef _DEBUG
		RegisterPressedEvent("develop mode", { {eKey::kJ, {	eAdditionalKey::kControl, eAdditionalKey::kShift }} },
			[this]()->bool { m_lobby_ui_layer->EnableDevelopMode(); return true; });
//#endif
		RegisterPressedEvent("volume up", { {eKey::kAdd} },
			[this]()->bool {client_fw::SoundManager::GetSoundManager().GetSound(0)->VolumeUp(0.1f); return true; });

		RegisterPressedEvent("volume down", { {eKey::kSubtract} },
			[this]()->bool {client_fw::SoundManager::GetSoundManager().GetSound(0)->VolumeDown(0.1f); return true; });
		
		RegisterPressedEvent("music stop", { {eKey::kS} },
			[this]()->bool {client_fw::SoundManager::GetSoundManager().GetSound(0)->Stop(); return true; });
		
		RegisterPressedEvent("music pause", { {eKey::kP} },
			[this]()->bool {client_fw::SoundManager::GetSoundManager().GetSound(0)->Pause(); return true; });

		return true;
	}

	void LobbyLevel::Shutdown()
	{
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
			//auto msg = std::static_pointer_cast<MatchingMessageOKEventInfo>(message);
			m_lobby_ui_layer->SucceededMatching();
			break;
		}
		case HashCode("sign in"):
		{
			//auto msg = std::static_pointer_cast<SignInMessageOkEventInfo>(message);
			m_lobby_ui_layer->SucceededLogin();
			break;
		}
		case HashCode("sign up"):
		{
			m_lobby_ui_layer->SucceededSignUp();
			break;
		}
		case HashCode("login fail"):
		{
			auto msg = std::static_pointer_cast<LoginFailMessageEventInfo>(message);
			auto reason = msg->GetLoginFailType();

			switch (reason)
			{
			case eLoginFailType::kAlreadyLogin:
				m_lobby_ui_layer->FailedLogin(eLoginFailState::kAlreadyLogin);
				break;
			case eLoginFailType::kInvalidPW:
			case eLoginFailType::kInvalidID:
				m_lobby_ui_layer->FailedLogin(eLoginFailState::kInvalidIDPW);
				break;
			}
			break;
		}
		default:
			break;
		}
	}

}