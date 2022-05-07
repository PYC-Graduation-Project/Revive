#include <include/client_core.h>
#include <client/object/ui/text_ui.h>
#include <client/object/ui/progress_bar_ui.h>
#include <client/renderer/core/render.h>

#include "player_info_ui_layer.h"
#include "object/actor/character/revive_player.h"

namespace revive
{
	PlayerInfoUILayer::PlayerInfoUILayer()
		: UserInterfaceLayer("player info ui layer")
	{
		Vec2 window_size = Render::GetWindowSize();

		m_hp_bar = CreateSPtr<ProgressBarUI>("player hp bar", Vec2(400.f, 32.f));
		m_hp_text = CreateSPtr<TextUI>("player hp text", Vec2(400.f, 32.f), L"30 / 30");
	}

	bool PlayerInfoUILayer::Initialize()
	{
		Vec2 window_size = Render::GetWindowSize();

		bool ret = GeneratePlayerUI(window_size);

		return ret;
	}

	void PlayerInfoUILayer::Update(float delta_time)
	{
	}

	bool PlayerInfoUILayer::GeneratePlayerUI(const Vec2& window_size)
	{
		bool ret = true;

		m_hp_bar->GetFillTexture()->SetBrushColor(Vec4(1.0f, 0.0f, 0.0f));
		m_hp_bar->SetPosition(Vec2(50.0f, 50.0f));
		m_hp_bar->SetPivot(Vec2(0.0f, 0.0f));
		ret &= RegisterUserInterface(m_hp_bar);

		m_hp_text->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);
		m_hp_text->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_hp_text->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		m_hp_text->SetFontName(L"배달의민족 주아");
		m_hp_text->SetFontSize(24);
		m_hp_text->SetPosition(Vec2(50.0f, 50.0f));
		m_hp_text->SetPivot(Vec2(0.0f, 0.0f));
		m_hp_text->SetColor(Vec4(0.f, 0.f, 0.f));
		ret &= RegisterUserInterface(m_hp_text);

		return ret;
	}

	void PlayerInfoUILayer::SetOwnerPlayer(const WPtr<RevivePlayer>& player)
	{
		m_revive_player = player;
		m_revive_player.lock()->OnChangedHPFunction([this](float hp, float max_hp)
			{
				m_hp_bar->SetPercent(hp / max_hp);
				m_hp_text->SetText(std::to_wstring(static_cast<INT>(hp)) + L" / " + std::to_wstring(static_cast<INT>(max_hp)));
			});
	}

	void PlayerInfoUILayer::RegisterOtherPlayer(const WPtr<DefaultPlayer>& player)
	{
		UINT id = player.lock()->GetNetworkID();
		std::string id_text = std::to_string(id);

		float y_offset = 50.0f * m_other_players_info.size();

		OtherPlayerUIInfo info;
		info.id_text = CreateSPtr<TextUI>(id_text + " player id text", Vec2(200.0f, 20.f), L"player - " + utf8_decode(player.lock()->GetPlayerName()));
		info.id_text->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);
		info.id_text->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
		info.id_text->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		info.id_text->SetFontName(L"배달의민족 주아");
		info.id_text->SetFontSize(16);
		info.id_text->SetPosition(Vec2(50.f, 150.0f + y_offset));
		info.id_text->SetPivot(Vec2(0.f, 1.f));
		info.id_text->SetColor(Vec4(1.f, 1.f, 1.f));
		RegisterUserInterface(info.id_text);

		info.hp_bar = CreateSPtr<ProgressBarUI>(id_text + " player hp bar", Vec2(200.f, 24.f));
		info.hp_bar->GetFillTexture()->SetBrushColor(Vec4(1.0f, 0.0f, 0.0f));
		info.hp_bar->SetPosition(Vec2(50.0f, 150.0f + y_offset));
		info.hp_bar->SetPivot(Vec2(0.0f, 0.0f));
		RegisterUserInterface(info.hp_bar);

		info.hp_text = CreateSPtr<TextUI>(id_text + " player hp text", Vec2(200.f, 24.f), L"30 / 30");
		info.hp_text->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);
		info.hp_text->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		info.hp_text->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		info.hp_text->SetFontName(L"배달의민족 주아");
		info.hp_text->SetFontSize(16);
		info.hp_text->SetPosition(Vec2(50.0f, 150.0f + y_offset));
		info.hp_text->SetPivot(Vec2(0.0f, 0.0f));
		info.hp_text->SetColor(Vec4(0.f, 0.f, 0.f));
		RegisterUserInterface(info.hp_text);

		info.other_player = player;

		m_other_players_info.emplace_back(std::move(info));

		const auto& player_info = *(m_other_players_info.cend() - 1);
		const auto& other_player = player_info.other_player.lock();
		other_player->OnChangedHPFunction([this, player_info](float hp, float max_hp)
			{
				player_info.hp_bar->SetPercent(hp / max_hp);
				player_info.hp_text->SetText(std::to_wstring(static_cast<INT>(hp)) + L" / " + std::to_wstring(static_cast<INT>(max_hp)));
			});
	}
}
