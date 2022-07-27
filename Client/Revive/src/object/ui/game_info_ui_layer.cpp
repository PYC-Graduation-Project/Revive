#include <include/client_core.h>
#include <client/object/ui/text_ui.h>
#include <client/object/ui/image_ui.h>
#include <client/renderer/core/render.h>

#include "game_info_ui_layer.h"
#include "object/ui/player_info_ui_layer.h"

#include "object/actor/gameplaymechanics/base.h"
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
			if (m_wave_count < 3 && m_wave_count >= 0)
			{
				float visible_time = m_next_wave_time - 10.f;
				if (visible_time < 1.f && visible_time > 0.f)
				{
					if(m_is_visible_next_wave_info == false)
						SetVisibleNextWaveInfo(true);
				}
			}
			if (m_wave_count < 3 && m_next_wave_time <= 0.f)
			{
				m_wave_count++;
				m_next_wave_time = 30.f;
			}
			info.wave_info_text->SetText(std::to_wstring(static_cast<INT>(m_next_wave_time)));
		};
		m_wave_ui_info.push_back(info);

		m_next_wave_bg_image = CreateSPtr<ImageUI>("next wave background image", Vec2{ 200.f,100.f });
		m_skeleton_king_count_text = CreateSPtr<TextUI>("skeleton king count", Vec2{ 100.f,50.f }, L"0");
		m_skeleton_soldier_count_text = CreateSPtr<TextUI>("skeleton soldier count", Vec2{ 100.f,50.f }, L"0");

		m_base_icon_image = CreateSPtr<ImageUI>("base icon image");
		for (auto& bar : m_base_hp_bar_bg_image)
			bar = CreateSPtr<ImageUI>("base hp bar background image");
		for (auto& bar : m_base_hp_bar_fg_image)
			bar = CreateSPtr<ImageUI>("base hp bar foreground image");
		SetVisibleNextWaveInfo(false);

	}

	bool GameInfoUILayer::Initialize()
	{
		Vec2 window_size = Render::GetWindowSize();

		bool ret = GenerateGameInfoUI(window_size);
		ret &= GenerateNextWaveInfoUI(window_size);

		return ret;
	}

	void GameInfoUILayer::Update(float delta_time)
	{
		for (auto wave_ui_info : m_wave_ui_info)
		{
			wave_ui_info.function(delta_time);
		}
		if (m_is_visible_next_wave_info)
		{
			m_next_wave_info_visible_time += delta_time;
			if (m_next_wave_info_visible_time > 10.f)
			{
				m_next_wave_info_visible_time = 0.f;
				SetVisibleNextWaveInfo(false);
			}
		}
		
	}

	bool GameInfoUILayer::GenerateGameInfoUI(const Vec2& window_size)
	{
		bool ret = true;

		auto SetGameInfoText = [this, &ret](const SPtr<TextUI> text,
			const Vec2& position)
		{
			text->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);
			text->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
			text->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
			text->SetFontName(L"kenvector_future");
			text->SetColor(Vec4(1.f, 1.f, 1.f));
			text->SetPivot(Vec2(0.f, 0.f));
			text->SetFontSize(24);
			text->SetPosition(position);
			ret &= RegisterUserInterface(text);
		};

		auto SetGameInfoImage = [this, &ret](const SPtr<ImageUI> image, 
			const Vec2& position, const std::string& image_path)
		{
			image->SetTexture(image_path);
			image->SetPivot(Vec2{ 0, 0 });
			image->SetPosition(position);
			ret &= RegisterUserInterface(image);
		};

		UINT offset_index = static_cast<UINT>(m_wave_ui_info.size() - 1);
		for (const auto& wave_ui_info : m_wave_ui_info)
		{
			auto bg_image = wave_ui_info.wave_info_bg_image;
			float bg_xoffset = 30.f + bg_image->GetSize().x;
			float bg_sizey = bg_image->GetSize().y;
			SetGameInfoImage(wave_ui_info.wave_info_bg_image, 
				Vec2{ window_size.x - bg_image->GetSize().x - bg_xoffset * offset_index--, 0.f }, wave_ui_info.bg_image_path);
			if (wave_ui_info.icon_image_path.empty() == false)
			{
				Vec2 icon_offset = (Vec2{ bg_sizey,bg_sizey } - wave_ui_info.wave_info_icon_image->GetSize()) / Vec2(2.0f, 2.0f);
				SetGameInfoImage(wave_ui_info.wave_info_icon_image, bg_image->GetPosition() + icon_offset, wave_ui_info.icon_image_path);
			}
			else bg_sizey = 0.f;
			SetGameInfoText(wave_ui_info.wave_info_text, bg_image->GetPosition() + Vec2{ bg_sizey , 0.f });
		}

		float pos_x = (window_size.x * 0.5f) - 200.f;
		float pos_y = 10.f;
		std::array <HPBarResourceInfo, 3> rsc_info =
		{
			"left.png",Vec2{pos_x - 6.f, pos_y},Vec2{6.f,32.f},
			"right.png",Vec2{pos_x + 388.f, pos_y},Vec2{6,32.f},
			"mid.png", Vec2{pos_x, pos_y},Vec2{388.f,32.f},
		};

		Vec2 bar_offset = Vec2{ 7.f,7.f };
		for (int i = 0; i < 3; ++i)
		{
			m_base_hp_bar_bg_image[i]->SetTexture("Contents/ui/barHorizontal_white_" + rsc_info[i].shape);
			m_base_hp_bar_bg_image[i]->SetPivot(Vec2(0.0f, 0.0f));
			m_base_hp_bar_bg_image[i]->SetPosition(rsc_info[i].position - bar_offset / Vec2{ 2.f,2.f });
			m_base_hp_bar_bg_image[i]->SetSize(rsc_info[i].size + bar_offset);
			m_base_hp_bar_fg_image[i]->SetTexture("Contents/ui/barHorizontal_red_" + rsc_info[i].shape);
			m_base_hp_bar_fg_image[i]->SetPivot(Vec2(0.0f, 0.0f));
			m_base_hp_bar_fg_image[i]->SetPosition(rsc_info[i].position);
			m_base_hp_bar_fg_image[i]->SetSize(rsc_info[i].size);
		}
		for (auto& bar : m_base_hp_bar_bg_image)
			ret &= RegisterUserInterface(bar);
		for (auto& bar : m_base_hp_bar_fg_image)
			ret &= RegisterUserInterface(bar);

		m_base_icon_image->SetTexture("Contents/ui/base_icon.dds");
		m_base_icon_image->SetPosition(Vec2(pos_x - 60.f,0.f));
		m_base_icon_image->SetSize(Vec2(50.f,50.f));
		m_base_icon_image->SetPivot(Vec2(0.f,0.f));
		ret &= RegisterUserInterface(m_base_icon_image);

		return ret;
	}

	bool GameInfoUILayer::GenerateNextWaveInfoUI(const Vec2& window_size)
	{
		bool ret = true;

		auto bg_imaage_position = window_size * 0.5f;
		bg_imaage_position.y -= m_next_wave_bg_image->GetSize().y * 1.5f;
		m_next_wave_bg_image->SetTexture("Contents/ui/next_wave_info.dds");
		m_next_wave_bg_image->SetPosition(bg_imaage_position);
		ret &= RegisterUserInterface(m_next_wave_bg_image);

		auto SetNextWaveText = [this, &ret](const SPtr<TextUI> text,
			const Vec2& position)
		{
			text->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);
			text->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
			text->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
			text->SetFontName(L"배달의민족 주아");
			text->SetColor(Vec4(1.f, 1.f, 1.f));
			text->SetFontSize(16);
			text->SetPosition(position);
			ret &= RegisterUserInterface(text);
		};
		
		auto bg_img_size = m_next_wave_bg_image->GetSize();
		SetNextWaveText(m_skeleton_king_count_text,
			m_next_wave_bg_image->GetPosition() - bg_img_size * 0.5f + Vec2{ bg_img_size.x * 0.8f ,bg_img_size.y * 0.35f });
		SetNextWaveText(m_skeleton_soldier_count_text,
			m_next_wave_bg_image->GetPosition() - bg_img_size * 0.5f + Vec2{ bg_img_size.x * 0.8f ,bg_img_size.y * 0.65f });

		

		return ret;
	}

	void GameInfoUILayer::SetNextWaveCount(const int soldier_num, const int king_num)
	{
		m_skeleton_king_count_text->SetText(std::to_wstring(king_num));
		m_skeleton_soldier_count_text->SetText(std::to_wstring(soldier_num));
		
	}

	void GameInfoUILayer::SetVisibleNextWaveInfo(bool value)
	{
		if (value)
		{
			m_next_wave_bg_image->SetVisible(true);
			m_skeleton_king_count_text->SetVisible(true);
			m_skeleton_soldier_count_text->SetVisible(true);
		}
		else
		{
			m_next_wave_bg_image->SetVisible(false);
			m_skeleton_king_count_text->SetVisible(false);
			m_skeleton_soldier_count_text->SetVisible(false);
		}
		m_is_visible_next_wave_info = value;
	}

	void GameInfoUILayer::RegisterEnemy(const WPtr<Enemy>& enemy)
	{
		m_mob_count++;
		enemy.lock()->OnMobCountDecrementFunction([this]()
		{
			m_mob_count--;
		});
	}

	void revive::GameInfoUILayer::RegisterBase(const WPtr<Base>& base)
	{
		if (base.expired() == false)
		{
			base.lock()->OnChangeHPFunction([this](float hp, float max_hp)
			{
				if (hp >= 0)
				{
					float size = 400 * hp / max_hp;

					if (m_base_hp_bar_fg_image[0] == nullptr)
						LOG_WARN("Base NullPtr");
					m_base_hp_bar_fg_image[0]->SetSize(Vec2{ std::clamp(size - 0.f, 0.f, 6.f),32.f });
					m_base_hp_bar_fg_image[1]->SetSize(Vec2{ std::clamp(size - 388.f, 0.f, 6.f),32.f });
					m_base_hp_bar_fg_image[2]->SetSize(Vec2{ std::clamp(size - 6.f, 0.f, 388.f),32.f });
				}
			});
		}
	}
}
