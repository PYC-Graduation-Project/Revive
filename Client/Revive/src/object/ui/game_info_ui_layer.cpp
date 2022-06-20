#include <include/client_core.h>
#include <client/object/ui/text_ui.h>
#include <client/object/ui/image_ui.h>
#include <client/renderer/core/render.h>

#include "game_info_ui_layer.h"
#include "object/actor/character/enemy.h"


namespace revive
{
	GameInfoUILayer::GameInfoUILayer()
		: UserInterfaceLayer("game info ui layer")
	{
		Vec2 window_size = Render::GetWindowSize();

		WaveUIInfo info;
		info.wave_info_bg_image = CreateSPtr<ImageUI>("wave info background image", Vec2{ 100.f,50.f });
		info.wave_info_text = CreateSPtr<TextUI>("wave info count", Vec2{ 100.f,50.f }, L"Wave ");
		info.bg_image_path = "Contents/ui/glassPanel.dds";
		info.function = [info, this](float delta_time) {
			if (m_wave_count > 0)
				info.wave_info_text->SetText(L"Wave " + std::to_wstring(static_cast<INT>(m_wave_count)));
			else
				info.wave_info_text->SetText(L"Ready");
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
			//m_next_wave_time -= delta_time;
			/*m_next_wave_time = std::clamp(m_next_wave_time, 0.f, 30.f);*/
			if (m_wave_count <= 3 && m_next_wave_time <= 0.f)
			{
				m_wave_count++;
				m_next_wave_time = 30.f;
			}
			info.wave_info_text->SetText(std::to_wstring(static_cast<INT>(m_next_wave_time)));
		};
		m_wave_ui_info.push_back(info);
	}
	bool GameInfoUILayer::Initialize()
	{
		Vec2 window_size = Render::GetWindowSize();

		bool ret = GenerateGameInfoUI(window_size);

		return ret;
	}
	void GameInfoUILayer::Update(float delta_time)
	{
		for (auto wave_ui_info : m_wave_ui_info)
		{
			wave_ui_info.function(delta_time);
		}
	}
	bool GameInfoUILayer::GenerateGameInfoUI(const Vec2& window_size)
	{
		bool ret = true;

		UINT offset_index = static_cast<UINT>(m_wave_ui_info.size() - 1);
		for (const auto& wave_ui_info : m_wave_ui_info)
		{
			auto bg_image = wave_ui_info.wave_info_bg_image;
			float xoffset = 30.f + bg_image->GetSize().x;
			bg_image->SetTexture(wave_ui_info.bg_image_path);
			bg_image->SetPivot(Vec2{ 0.f,0.f });
			Vec2 bg_position = Vec2{ window_size.x - bg_image->GetSize().x - xoffset * offset_index--, 0.f };
			bg_image->SetPosition(bg_position);
			ret &= RegisterUserInterface(bg_image);
			float bg_sizey = bg_image->GetSize().y;
			if (wave_ui_info.icon_image_path.empty() == false)
			{
				auto icon_image = wave_ui_info.wave_info_icon_image;
				icon_image->SetTexture(wave_ui_info.icon_image_path);
				icon_image->SetPivot(Vec2{ 0.0f, 0.0f });
				Vec2 icon_offset = (Vec2{ bg_sizey,bg_sizey } - icon_image->GetSize()) / Vec2(2.0f, 2.0f);
				Vec2 icon_pos = bg_position + icon_offset;
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
			text->SetPosition(bg_image->GetPosition() + Vec2{ bg_sizey , 0.f });
			text->SetPivot(Vec2(0.0f, 0.0f));
			text->SetColor(Vec4(1.f, 1.f, 1.f));
			ret &= RegisterUserInterface(text);
		}

		return ret;
	}
	void GameInfoUILayer::RegisterEnemy(const WPtr<Enemy>& enemy)
	{
		m_mob_count++;
		enemy.lock()->OnMobCountDecrementFunction([this]()
		{
			m_mob_count--;
		});
	}
}
