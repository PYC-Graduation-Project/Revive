#include <include/client_core.h>
#include <client/object/ui/button_ui.h>
#include <client/object/ui/image_ui.h>
#include <client/renderer/core/render.h>
#include <client/event/packetevent/packet_helper.h>
#include <client/object/level/core/level_manager.h>
#include <client/object/level/core/level_loader.h>

#include "object/ui/game_end_ui_layer.h"
#include "object/level/lobby_level.h"

namespace revive
{
	GameEndUILayer::GameEndUILayer()
		: UserInterfaceLayer("game end ui layer")
	{
		m_end_game_image = CreateSPtr<ImageUI>("end game image", Vec2(400.0f, 400.0f));
		m_to_lobby_button = CreateSPtr<ButtonUI>("to lobby button");
	}

	bool GameEndUILayer::Initialize()
	{
		Vec2 window_size = Render::GetWindowSize();
		bool ret = true;

		switch (m_game_state)
		{
		case eGameState::kWin:
			m_end_game_image->SetTexture("Contents/ui/win_menu.dds");
			m_lobby_button_enable_time = 3.0f;
			break;
		case eGameState::kDefeat:
			m_end_game_image->SetTexture("Contents/ui/defeat_menu.dds");
			m_lobby_button_enable_time = 3.0f;
			break;
		case eGameState::kRun:
			m_end_game_image->SetTexture("Contents/ui/exit_game_menu.png");
			m_lobby_button_enable_time = 0.0f;
			break;
		default:
			break;
		}

		m_end_game_image->SetPosition(window_size * 0.5f);
		ret &= RegisterUserInterface(m_end_game_image);

		m_to_lobby_button->SetNormalTexture("Contents/ui/to_lobby_normal.dds");
		m_to_lobby_button->SetHoveredTexture("Contents/ui/to_lobby_hovered.dds");
		m_to_lobby_button->SetPressedTexture("Contents/ui/to_lobby_pressed.dds");
		m_to_lobby_button->SetPosition(window_size * 0.5f + Vec2(0.0f, 120.0f));
		m_to_lobby_button->SetSize(Vec2(300.0f, 120.0f));
		m_to_lobby_button->OnClicked([this]() {
			LevelManager::GetLevelManager().OpenLevel(CreateSPtr<LobbyLevel>(), nullptr);
			});

		switch (m_game_state)
		{
		case eGameState::kWin:
		case eGameState::kDefeat:
			m_to_lobby_button->SetVisible(false);
			m_to_lobby_button->SetActivate(false);
			break;
		default:
			break;
		}

		ret &= RegisterUserInterface(m_to_lobby_button);

		return ret;
	}

	void GameEndUILayer::Update(float delta_time)
	{
		if (m_lobby_button_enable_time > 0.0f)
		{
			m_lobby_button_enable_time -= delta_time;
			if (m_lobby_button_enable_time <= 0.0f)
			{
				m_to_lobby_button->SetActivate(true);
				m_to_lobby_button->SetVisible(true);
			}
		}
	}
}
