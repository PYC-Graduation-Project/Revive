#include <include/client_core.h>
#include <client/object/ui/text_ui.h>
#include <client/object/ui/image_ui.h>
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

		//m_hp_bar = CreateSPtr<ProgressBarUI>("player hp bar", Vec2(00.f, 32.f));
		for(auto& bar :m_hp_bar_bg)
			bar = CreateSPtr<ImageUI>("player hp bar background image");
		for(auto& bar : m_hp_bar_fg)
			bar = CreateSPtr<ImageUI>("player hp bar foreground image");
		m_hp_text = CreateSPtr<TextUI>("player hp text", Vec2(400.f, 32.f), L"30 / 30");
		m_crosshair_image = CreateSPtr<ImageUI>("crossair", Vec2(32.f,32.f));

		WaveUIInfo info;
		info.wave_info_bg_image = CreateSPtr<ImageUI>("wave info background image", Vec2{ 100.f,50.f });
		info.wave_info_text = CreateSPtr<TextUI>("wave info count", Vec2{ 100.f,50.f }, L"Wave ");
		info.bg_image_path = "Contents/ui/glassPanel.dds";
		info.function = [info,this](float delta_time) {
			info.wave_info_text->SetText(L"Wave " + std::to_wstring(static_cast<INT>(m_wave_count)));
		};
		m_wave_ui_info.push_back(info);

		info.wave_info_bg_image = CreateSPtr<ImageUI>("wave info background image", Vec2{ 100.f,50.f });
		info.wave_info_icon_image = CreateSPtr<ImageUI>("wave info mob icon image", Vec2{ 30.f,30.f });
		info.wave_info_text = CreateSPtr<TextUI>("wave info mob count", Vec2{ 50.f,50.f }, L"0");
		info.bg_image_path = "Contents/ui/glassPanel.dds";
		info.icon_image_path = "Contents/ui/skull.dds";
		info.function = [info, this](float delta_time) {
			info.wave_info_text->SetText(std::to_wstring(static_cast<INT>(m_mob_count)));
		};
		m_wave_ui_info.push_back(info);

		info.wave_info_bg_image = CreateSPtr<ImageUI>("wave info background image", Vec2{ 100.f,50.f });
		info.wave_info_icon_image = CreateSPtr<ImageUI>("wave info time icon image", Vec2{ 30.f,30.f });
		info.wave_info_text = CreateSPtr<TextUI>("wave info time", Vec2{ 50.f,50.f }, L"0");
		info.bg_image_path = "Contents/ui/glassPanel.dds";
		info.icon_image_path = "Contents/ui/hourglass.dds";
		info.function = [info, this](float delta_time) {
			m_next_wave_time -= delta_time;
			m_next_wave_time = std::clamp(m_next_wave_time, 0.f, 30.f);
			if (m_wave_count <= 3 && m_next_wave_time <= 0.f)
			{
				m_wave_count++;
				m_next_wave_time = 30.f;
			}
			info.wave_info_text->SetText(std::to_wstring(static_cast<INT>(m_next_wave_time)));
		};
		m_wave_ui_info.push_back(info);

	}

	bool PlayerInfoUILayer::Initialize()
	{
		Vec2 window_size = Render::GetWindowSize();

		bool ret = GeneratePlayerUI(window_size);

		return ret;
	}

	void PlayerInfoUILayer::Update(float delta_time)
	{
		for (auto wave_ui_info : m_wave_ui_info)
		{
			wave_ui_info.function(delta_time);
		}

	}

	bool PlayerInfoUILayer::GeneratePlayerUI(const Vec2& window_size)
	{
		bool ret = true;

		std::array <HPBarResourceInfo, 3> rsc_info =
		{
			"left.png",Vec2{50.f - 6.f, 50.0f},Vec2{6.f,32.f},
			"right.png",Vec2{50.f + 388.f, 50.0f},Vec2{6,32.f},
			"mid.png", Vec2{50.f, 50.0f},Vec2{388.f,32.f},
		};

		Vec2 bar_offset = Vec2{ 7.f,7.f };
		for (int i = 0; i < 3; ++i)
		{
			m_hp_bar_bg[i]->SetTexture("Contents/ui/barHorizontal_white_" + rsc_info[i].shape);
			m_hp_bar_bg[i]->SetPivot(Vec2(0.0f, 0.0f));
			m_hp_bar_bg[i]->SetPosition(rsc_info[i].position - bar_offset / 2);
			m_hp_bar_bg[i]->SetSize(rsc_info[i].size + bar_offset);
			m_hp_bar_fg[i]->SetTexture("Contents/ui/barHorizontal_red_" + rsc_info[i].shape);
			m_hp_bar_fg[i]->SetPivot(Vec2(0.0f, 0.0f));
			m_hp_bar_fg[i]->SetPosition(rsc_info[i].position);
			m_hp_bar_fg[i]->SetSize(rsc_info[i].size);
		}
		for (auto& bar : m_hp_bar_bg)
			ret &= RegisterUserInterface(bar);
		for (auto& bar : m_hp_bar_fg)
			ret &= RegisterUserInterface(bar);

		/*m_hp_bar->GetFillTexture()->SetBrushColor(Vec4(1.0f, 0.0f, 0.0f));
		m_hp_bar->SetPosition(Vec2(50.0f, 50.0f));
		m_hp_bar->SetPivot(Vec2(0.0f, 0.0f));*/

		m_hp_text->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);
		m_hp_text->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_hp_text->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		m_hp_text->SetFontName(L"배달의민족 주아");
		m_hp_text->SetFontSize(24);
		m_hp_text->SetPosition(Vec2(50.0f, 50.0f));
		m_hp_text->SetPivot(Vec2(0.0f, 0.0f));
		m_hp_text->SetColor(Vec4(0.f, 0.f, 0.f));
		ret &= RegisterUserInterface(m_hp_text);

		UINT offset_index = static_cast<UINT>(m_wave_ui_info.size() - 1);
		for (const auto& wave_ui_info : m_wave_ui_info)
		{
			auto bg_image = wave_ui_info.wave_info_bg_image;
			float xoffset = 30.f + bg_image->GetSize().x;
			bg_image->SetTexture(wave_ui_info.bg_image_path);
			bg_image->SetPivot(Vec2{ 0.f,0.f });
			bg_image->SetPosition(Vec2{ window_size.x - bg_image->GetSize().x - xoffset * offset_index--, 0.f });
			ret &= RegisterUserInterface(bg_image);
			float bg_sizey = bg_image->GetSize().y;
			if (wave_ui_info.icon_image_path.empty() == false)
			{
				auto icon_image = wave_ui_info.wave_info_icon_image;
				icon_image->SetTexture(wave_ui_info.icon_image_path);
				icon_image->SetPivot(Vec2{ 0.f,0.f });
				Vec2 icon_pos = bg_image->GetPosition() + (Vec2{ bg_sizey,bg_sizey } - icon_image->GetSize()) / 2.0f;
				icon_image->SetPosition(icon_pos);
				ret &= RegisterUserInterface(icon_image);
			}
			else bg_sizey = 0.f;
			auto text = wave_ui_info.wave_info_text;
			text->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);
			text->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
			text->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
			text->SetFontName(L"kenvector_future");
			text->SetFontSize(24);
			text->SetPosition(bg_image->GetPosition() + Vec2{ bg_sizey , 0.f});
			text->SetPivot(Vec2(0.0f, 0.0f));
			text->SetColor(Vec4(1.f, 1.f, 1.f));
			ret &= RegisterUserInterface(text);
		}

		m_crosshair_image->SetTexture("Contents/ui/crossair_white.dds");
		m_crosshair_image->SetPosition(window_size * 0.5f);
		ret &= RegisterUserInterface(m_crosshair_image);
		
		return ret;
	}

	void PlayerInfoUILayer::SetOwnerPlayer(const WPtr<RevivePlayer>& player)
	{
		m_revive_player = player;
		m_revive_player.lock()->OnChangedHPFunction([this](float hp, float max_hp)
			{
				float size = 400 * hp / max_hp;
				
				m_hp_bar_fg[0]->SetSize(Vec2{ std::clamp(size - 0.f, 0.f, 6.f),32.f });
				m_hp_bar_fg[1]->SetSize(Vec2{ std::clamp(size - 388.f, 0.f, 6.f),32.f });
				m_hp_bar_fg[2]->SetSize(Vec2{ std::clamp(size - 6.f, 0.f, 388.f),32.f });

				//m_hp_bar->SetPercent(hp / max_hp)				
;
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

		/*info.hp_bar = CreateSPtr<ProgressBarUI>(id_text + " player hp bar", Vec2(200.f, 24.f));
		info.hp_bar->GetFillTexture()->SetBrushColor(Vec4(1.0f, 0.0f, 0.0f));
		info.hp_bar->SetPosition(Vec2(50.0f, 150.0f + y_offset));
		info.hp_bar->SetPivot(Vec2(0.0f, 0.0f));
		RegisterUserInterface(info.hp_bar);*/

		std::array <HPBarResourceInfo, 3> rsc_info =
		{
			"left.png",Vec2{50.f - 3.f, 150.0f + y_offset},Vec2{3.f,24.f},
			"right.png",Vec2{50.f + 194.f, 150.0f + y_offset},Vec2{3,24.f},
			"mid.png", Vec2{50.f, 150.0f + y_offset},Vec2{194.f,24.f},
		};

		Vec2 bar_bg_offset = Vec2{ 5.f,5.f};
		for (int i = 0; i < 3; ++i)
		{
			info.hp_bar_bg[i] = CreateSPtr<ImageUI>(id_text + " player hp bar bg");
			info.hp_bar_bg[i]->SetTexture("Contents/ui/barHorizontal_white_" + rsc_info[i].shape);
			info.hp_bar_bg[i]->SetPivot(Vec2(0.0f, 0.0f));
			info.hp_bar_bg[i]->SetPosition(rsc_info[i].position - bar_bg_offset / 2);
			info.hp_bar_bg[i]->SetSize(rsc_info[i].size + bar_bg_offset);

			info.hp_bar_fg[i] = CreateSPtr<ImageUI>(id_text + " player hp bar fg");
			info.hp_bar_fg[i]->SetTexture("Contents/ui/barHorizontal_red_" + rsc_info[i].shape);
			info.hp_bar_fg[i]->SetPivot(Vec2(0.0f, 0.0f));
			info.hp_bar_fg[i]->SetPosition(rsc_info[i].position);
			info.hp_bar_fg[i]->SetSize(rsc_info[i].size);
		}
		for (auto& bar : info.hp_bar_bg)
			RegisterUserInterface(bar);
		for (auto& bar : info.hp_bar_fg)
			RegisterUserInterface(bar);

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
				float size = 200 * hp / max_hp;

				player_info.hp_bar_fg[0]->SetSize(Vec2{ std::clamp(size - 0.f, 0.f, 3.f),24.f });
				player_info.hp_bar_fg[1]->SetSize(Vec2{ std::clamp(size - 194.f, 0.f, 3.f),24.f });
				player_info.hp_bar_fg[2]->SetSize(Vec2{ std::clamp(size - 3.f, 0.f, 194.f),24.f });
				//player_info.hp_bar->SetPercent(hp / max_hp);
				player_info.hp_text->SetText(std::to_wstring(static_cast<INT>(hp)) + L" / " + std::to_wstring(static_cast<INT>(max_hp)));
			});
	}
}
