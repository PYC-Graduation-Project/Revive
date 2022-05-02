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
	GameEndUILayer::GameEndUILayer(eGameResult game_result)
		: UserInterfaceLayer("game end ui layer")
		, m_game_result(game_result)
	{
		m_end_game_image = CreateSPtr<ImageUI>("end game image", Vec2(400.0f, 400.0f));
		m_to_lobby_button = CreateSPtr<ButtonUI>("to lobby button");
	}

	bool GameEndUILayer::Initialize()
	{
		Vec2 window_size = Render::GetWindowSize();
		bool ret = true;

		if (m_game_result == eGameResult::kWin)
			m_end_game_image->SetTexture("Contents/ui/win_menu.dds");
		else
			m_end_game_image->SetTexture("Contents/ui/defeat_menu.dds");

		m_end_game_image->SetPosition(window_size * 0.5f);
		ret &= RegisterUserInterface(m_end_game_image);

		m_to_lobby_button->SetNormalTexture("Contents/ui/ok_normal.dds");
		m_to_lobby_button->SetHoveredTexture("Contents/ui/ok_hovered.dds");
		m_to_lobby_button->SetPressedTexture("Contents/ui/ok_pressed.dds");
		m_to_lobby_button->SetPosition(window_size * 0.5f + Vec2(0.0f, 120.0f));
		m_to_lobby_button->SetSize(Vec2(300.0f, 120.0f));
		m_to_lobby_button->OnClicked([this]() {
			LevelManager::GetLevelManager().OpenLevel(CreateSPtr<LobbyLevel>(), nullptr);
			});
		m_to_lobby_button->SetVisible(false);
		m_to_lobby_button->SetActivate(false);
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
