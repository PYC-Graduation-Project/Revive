#include <include/client_core.h>
#include <client/object/ui/text_ui.h>
#include <client/object/ui/button_ui.h>
#include <client/object/ui/image_ui.h>
#include <client/object/ui/text_box_ui.h>
#include <client/renderer/core/render.h>
#include <client/event/packetevent/packet_helper.h>
#include <client/object/level/core/level_manager.h>
#include <client/object/level/core/level_loader.h>
#include <client/asset/sound/core/sound_manager.h>
#include <client/core/application.h>

#include "revive_server/message/message_event_info.h"
#include "object/ui/lobby_ui_layer.h"
#include "object/level/game_play_level.h"
#include "object/level/sharedinfo/revive_level_shared_info.h"

namespace revive
{
	LobbyUILayer::LobbyUILayer()
		: UserInterfaceLayer("lobby ui layer")
	{
		Vec2 window_size = Render::GetWindowSize();
		m_game_option_state = std::static_pointer_cast<ReviveLevelSharedInfo>(LevelManager::GetLevelManager().GetLevelSharedInfo())->GetGameOption();

		m_title_image = CreateSPtr<ImageUI>("game title image", Vec2(525.f,150.f));
		m_login_bg_image = CreateSPtr<ImageUI>("login background image", Vec2(550.f, 270.f));
		m_id_text_box = CreateSPtr<TextBoxUI>("id text box", Vec2(420.0f, 40.0f));
		m_pw_text_box = CreateSPtr<TextBoxUI>("pw text box", Vec2(420.0f, 40.0f));
		m_sign_up_button = CreateSPtr<ButtonUI>("sign up button");
		m_sign_in_button = CreateSPtr<ButtonUI>("sign in button");
		m_matching_button = CreateSPtr<ButtonUI>("matching button");
		m_num_of_user_text = CreateSPtr<TextUI>("num of user text", Vec2(200.0f, 30.0f), L"매칭 인원 : " + std::to_wstring(m_num_of_user));
		m_user_change_button = CreateSPtr<ButtonUI>("user change button");

		m_invalid_id_pw_image = CreateSPtr<ImageUI>("invalid id pw image", Vec2(400.0f, 400.0f));
		m_invalid_id_pw_ok_button = CreateSPtr<ButtonUI>("invalid id pw ok button");
		m_already_logged_in_image = CreateSPtr<ImageUI>("already logged in image", Vec2(400.0f, 400.0f));
		m_already_logged_in_ok_button = CreateSPtr<ButtonUI>("already logged in ok button");
		m_login_succeed_image = CreateSPtr<ImageUI>("login succeed image", Vec2(400.0f, 400.0f));
		m_login_succeed_ok_button = CreateSPtr<ButtonUI>("login succeed ok button");

		m_not_login_image = CreateSPtr<ImageUI>("no login image", Vec2(400.0f, 400.0f));
		m_not_login_ok_button = CreateSPtr<ButtonUI>("not login ok button");
		m_matching_image = CreateSPtr<ImageUI>("matching image", Vec2(400.0f, 400.0f));
		m_matching_cancel_button = CreateSPtr<ButtonUI>("matching ok button");
		m_matching_time_text = CreateSPtr<TextUI>("matching time text", Vec2(350.0f, 50.0f), L"매칭 시간 : 0 : 0");
		m_matching_success_image = CreateSPtr<ImageUI>("matching success iamge", Vec2(400.0f, 400.0f));
		m_game_start_button = CreateSPtr<ButtonUI>("game start button");

		m_close_app_button = CreateSPtr<ButtonUI>("close app button");
		m_develop_mode_button = CreateSPtr<ButtonUI>("develop mode button");
		m_option_button = CreateSPtr<ButtonUI>("option button");

		{
			m_option_menu_image = CreateSPtr<ImageUI>("option menu image", Vec2(window_size.y - 50.0f, window_size.y - 50.0f));
			Vec2 option_size = m_option_menu_image->GetSize();

			m_option_graphic_button = CreateSPtr<ButtonUI>("graphic option menu button");
			m_option_sound_button = CreateSPtr<ButtonUI>("sound option menu button");
			m_graphic_option_shadow_enable_text = CreateSPtr<TextUI>("graphic option shadow enable text", Vec2(option_size.x * 0.3f, option_size.x / 12.0f), L"그림자 활성화");
			m_graphic_option_shadow_quality_text = CreateSPtr<TextUI>("graphic option shadow quality text", Vec2(option_size.x * 0.3f, option_size.x / 12.0f), L"그림자 품질");
			m_sound_option_bgm_volume_text = CreateSPtr<TextUI>("sound option bgm volume text", Vec2(option_size.x * 0.3f, option_size.x / 12.0f), L"배경음");
			m_sound_option_effect_volume_text = CreateSPtr<TextUI>("sound option effect volume text", Vec2(option_size.x * 0.3f, option_size.x / 12.0f), L"효과음");
			m_close_option_menu_button = CreateSPtr<ButtonUI>("close option menu button");
			m_shadow_enable_left_button = CreateSPtr<ButtonUI>("shadow enable left button");
			m_shadow_enable_right_button = CreateSPtr<ButtonUI>("shadow enable right button");
			m_shadow_enable_text = CreateSPtr<TextUI>("shadow enable text", Vec2(option_size.x * 0.3f, option_size.x / 12.0f), L"켜기");
			m_shadow_quality_left_button = CreateSPtr<ButtonUI>("shadow quality left button");
			m_shadow_quality_right_button = CreateSPtr<ButtonUI>("shadow quality right button");
			m_shadow_quality_text = CreateSPtr<TextUI>("shadow quality text", Vec2(option_size.x * 0.3f, option_size.x / 12.0f), L"고");
			m_bgm_volume_text = CreateSPtr<TextUI>("bgm volume text", Vec2(option_size.x * 0.3f, option_size.x / 12.0f), L"100");
			m_bgm_volume_left_button = CreateSPtr<ButtonUI>("bgm volume left button");
			m_bgm_volume_right_button = CreateSPtr<ButtonUI>("bgm volume right button");
			m_effect_volume_text = CreateSPtr<TextUI>("effect volume text", Vec2(option_size.x * 0.3f, option_size.x / 12.0f), L"100");
			m_effect_volume_left_button = CreateSPtr<ButtonUI>("effect volume left button");
			m_effect_volume_right_button = CreateSPtr<ButtonUI>("effect volume right button");
		}
		
		m_sign_up_succeed_image = CreateSPtr<ImageUI>("sign up image",Vec2(400.0f, 400.0f));
		m_sign_up_succeed_ok_button = CreateSPtr<ButtonUI>("sign up ok button");
		m_id_text = CreateSPtr<TextUI>("id text", Vec2(350.0f, 50.0f), L"ID");
		m_pw_text = CreateSPtr<TextUI>("pw text", Vec2(350.0f, 50.0f), L"PW");
	}

	bool LobbyUILayer::Initialize()
	{
	

		Vec2 window_size = Render::GetWindowSize();

		bool ret = GenerateMatchingUI(window_size);
		ret &= GenerateSignInMenuUI(window_size);
		ret &= GenerateMatchingMenuUI(window_size);
		ret &= GenerateOptionUI(window_size);
		ret &= GenrateSignUpUI(window_size);

		return ret;
	}

	void LobbyUILayer::Update(float delta_time)
	{
		if (m_is_wait_matching)
		{
			m_matching_time += delta_time;
			m_matching_time_text->SetText(L"매칭 시간 [ " + std::to_wstring(static_cast<UINT>(m_matching_time) / 60) +
				L" : " + std::to_wstring(static_cast<UINT>(m_matching_time) % 60) + L" ]");
		}
	}

	bool LobbyUILayer::GenerateMatchingUI(const Vec2& window_size)
	{
		bool ret = true;

		m_title_image->SetTexture("Contents/ui/game_title_image.dds");
		m_title_image->SetPosition(window_size * 0.5f + Vec2(0.0f, -350.0f));
		ret &= RegisterUserInterface(m_title_image);

		m_login_bg_image->SetTexture("Contents/ui/login_bg.dds");
		m_login_bg_image->SetPosition(window_size * 0.5f + Vec2(0.0f, 210.0f));
		ret &= RegisterUserInterface(m_login_bg_image);

		m_id_text_box->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);
		m_id_text_box->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_id_text_box->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		m_id_text_box->SetFontSize(15);
		m_id_text_box->SetPosition(window_size * 0.5f + Vec2(20.0f, 161.0f));
		m_id_text_box->SetBackgroundTexture("Contents/ui/id_pw_text_box.dds");
		m_id_text_box->OnCommitted([this]() {
			m_id = m_id_text_box->GetText();
			});
		ret &= RegisterUserInterface(m_id_text_box);

		m_pw_text_box->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);
		m_pw_text_box->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_pw_text_box->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		m_pw_text_box->SetFontSize(15);
		m_pw_text_box->SetPosition(window_size * 0.5f + Vec2(20.0f, 201.0f));
		m_pw_text_box->SetBackgroundTexture("Contents/ui/id_pw_text_box.dds");
		m_pw_text_box->OnCommitted([this]() {
			m_pw = m_pw_text_box->GetText();
			});
		ret &= RegisterUserInterface(m_pw_text_box);


		m_sign_up_button->SetNormalTexture("Contents/ui/sign_up_normal.dds");
		m_sign_up_button->SetHoveredTexture("Contents/ui/sign_up_hovered.dds");
		m_sign_up_button->SetPressedTexture("Contents/ui/sign_up_pressed.dds");
		m_sign_up_button->SetPosition(window_size * 0.5f + Vec2(160.0f, 270.0f));
		m_sign_up_button->SetSize(Vec2(150.0f, 60.0f));
		m_sign_up_button->OnClicked([this]() {
			PacketHelper::RegisterPacketEventToServer(CreateSPtr<SignUpMessageEventInfo>(HashCode("send sign up"), m_id.data(), m_pw.data()));
			});
		ret &= RegisterUserInterface(m_sign_up_button);

		m_sign_in_button->SetNormalTexture("Contents/ui/sign_in_normal.dds");
		m_sign_in_button->SetHoveredTexture("Contents/ui/sign_in_hovered.dds");
		m_sign_in_button->SetPressedTexture("Contents/ui/sign_in_pressed.dds");
		m_sign_in_button->SetPosition(window_size * 0.5f + Vec2(0.0f, 270.0f));
		m_sign_in_button->SetSize(Vec2(150.0f, 60.0f));
		m_sign_in_button->OnClicked([this]() {
			PacketHelper::RegisterPacketEventToServer(CreateSPtr<SignInMessageEventInfo>(HashCode("send sign in"), m_id.data(), m_pw.data()));
			});
		ret &= RegisterUserInterface(m_sign_in_button);


		m_matching_button->SetNormalTexture("Contents/ui/matching_normal.dds");
		m_matching_button->SetHoveredTexture("Contents/ui/matching_hovered.dds");
		m_matching_button->SetPressedTexture("Contents/ui/matching_pressed.dds");
		m_matching_button->SetPosition(window_size * 0.5f + Vec2(-160.0f, 270.0f));
		m_matching_button->SetSize(Vec2(150.0f, 60.0f));
		m_matching_button->OnClicked([this]() {
			if (m_succeeded_login)
			{
				PacketHelper::RegisterPacketEventToServer(CreateSPtr<MatchingMessageEventInfo>(HashCode("send sign matching"), m_num_of_user));
				m_is_wait_matching = true;
				EnablePopUpState(eLobbyPopupMenu::kMatching);
				m_matching_time = 0.0f;
			}
			else
			{
				EnablePopUpState(eLobbyPopupMenu::kNotLogin);
			}});
		ret &= RegisterUserInterface(m_matching_button);

		m_num_of_user_text->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);
		m_num_of_user_text->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
		m_num_of_user_text->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		m_num_of_user_text->SetFontName(L"배달의민족 주아");
		//m_num_of_user_text->SetColor(Vec4(236.f, 76.f, 122.f, 255.0f) / 255.f);
		m_num_of_user_text->SetFontSize(24);
		m_num_of_user_text->SetPosition(window_size * 0.5f + Vec2(0.0f, 380.0f));
		ret &= RegisterUserInterface(m_num_of_user_text);

		m_user_change_button->SetNormalTexture("Contents/ui/user_num_button.dds");
		m_user_change_button->SetHoveredTexture("Contents/ui/user_num_button_hovered.dds");
		m_user_change_button->SetPressedTexture("Contents/ui/user_num_button_pressed.dds");
		m_user_change_button->SetPosition(window_size * 0.5f + Vec2(80.0f, 380.0f));
		m_user_change_button->SetSize(Vec2(50.0f, 50.0f));
		m_user_change_button->OnClicked([this]() {
			++m_num_of_user;
			if (m_num_of_user == 4)
				m_num_of_user = 2;
			m_num_of_user_text->SetText(L"매칭 인원 : " + std::to_wstring(m_num_of_user));
			});
		ret &= RegisterUserInterface(m_user_change_button);

		return ret;
	}

	bool LobbyUILayer::GenerateSignInMenuUI(const Vec2& window_size)
	{
		bool ret = true;

		m_invalid_id_pw_image->SetTexture("Contents/ui/invalid_id_pw_menu.dds");
		m_invalid_id_pw_image->SetPosition(window_size * 0.5f);
		ret &= RegisterUserInterface(m_invalid_id_pw_image);

		m_invalid_id_pw_ok_button->SetNormalTexture("Contents/ui/ok_normal.dds");
		m_invalid_id_pw_ok_button->SetHoveredTexture("Contents/ui/ok_hovered.dds");
		m_invalid_id_pw_ok_button->SetPressedTexture("Contents/ui/ok_pressed.dds");
		m_invalid_id_pw_ok_button->SetPosition(window_size * 0.5f + Vec2(0.0f, 120.0f));
		m_invalid_id_pw_ok_button->SetSize(Vec2(300.0f, 120.0f));
		m_invalid_id_pw_ok_button->OnClicked([this]() {
			DisablePopUpState(eLobbyPopupMenu::kInvalidIDPW);
			});
		DisablePopUpState(eLobbyPopupMenu::kInvalidIDPW);
		ret &= RegisterUserInterface(m_invalid_id_pw_ok_button);

		m_already_logged_in_image->SetTexture("Contents/ui/already_logged_in_menu.dds");
		m_already_logged_in_image->SetPosition(window_size * 0.5f);
		ret &= RegisterUserInterface(m_already_logged_in_image);

		m_already_logged_in_ok_button->SetNormalTexture("Contents/ui/ok_normal.dds");
		m_already_logged_in_ok_button->SetHoveredTexture("Contents/ui/ok_hovered.dds");
		m_already_logged_in_ok_button->SetPressedTexture("Contents/ui/ok_pressed.dds");
		m_already_logged_in_ok_button->SetPosition(window_size * 0.5f + Vec2(0.0f, 120.0f));
		m_already_logged_in_ok_button->SetSize(Vec2(300.0f, 120.0f));
		m_already_logged_in_ok_button->OnClicked([this]() {
			DisablePopUpState(eLobbyPopupMenu::kAlreadyLogin);
			});
		DisablePopUpState(eLobbyPopupMenu::kAlreadyLogin);
		ret &= RegisterUserInterface(m_already_logged_in_ok_button);

		m_login_succeed_image->SetTexture("Contents/ui/log_in_succeed_menu.dds");
		m_login_succeed_image->SetPosition(window_size * 0.5f);
		ret &= RegisterUserInterface(m_login_succeed_image);

		m_login_succeed_ok_button->SetNormalTexture("Contents/ui/ok_normal.dds");
		m_login_succeed_ok_button->SetHoveredTexture("Contents/ui/ok_hovered.dds");
		m_login_succeed_ok_button->SetPressedTexture("Contents/ui/ok_pressed.dds");
		m_login_succeed_ok_button->SetPosition(window_size * 0.5f + Vec2(0.0f, 120.0f));
		m_login_succeed_ok_button->SetSize(Vec2(300.0f, 120.0f));
		m_login_succeed_ok_button->OnClicked([this]() {
			DisablePopUpState(eLobbyPopupMenu::kLoginSuccess);
			});
		DisablePopUpState(eLobbyPopupMenu::kLoginSuccess);
		ret &= RegisterUserInterface(m_login_succeed_ok_button);
		

		return ret;
	}

	bool LobbyUILayer::GenerateMatchingMenuUI(const Vec2& window_size)
	{
		bool ret = true;

		m_not_login_image->SetTexture("Contents/ui/log_in_before_matching_menu.dds");
		m_not_login_image->SetPosition(window_size * 0.5f );
		ret &= RegisterUserInterface(m_not_login_image);

		m_not_login_ok_button->SetNormalTexture("Contents/ui/ok_normal.dds");
		m_not_login_ok_button->SetHoveredTexture("Contents/ui/ok_hovered.dds");
		m_not_login_ok_button->SetPressedTexture("Contents/ui/ok_pressed.dds");
		m_not_login_ok_button->SetPosition(window_size * 0.5f + Vec2(0.0f, 120.0f));
		m_not_login_ok_button->SetSize(Vec2(300.0f, 120.0f));
		m_not_login_ok_button->OnClicked([this]() {
			DisablePopUpState(eLobbyPopupMenu::kNotLogin);
			});
		DisablePopUpState(eLobbyPopupMenu::kNotLogin);
		ret &= RegisterUserInterface(m_not_login_ok_button);

		m_matching_image->SetTexture("Contents/ui/mathcing_menu.dds");
		m_matching_image->SetPosition(window_size * 0.5f);
		ret &= RegisterUserInterface(m_matching_image);

		m_matching_time_text->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);
		m_matching_time_text->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_matching_time_text->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		m_matching_time_text->SetFontSize(40);
		m_matching_time_text->SetColor(Vec4(239.0f / 255.0f, 71.0f / 255.0f, 84.0f / 255.0f, 1.0f));
		m_matching_time_text->SetPosition(window_size * 0.5f - Vec2(0.0f, 10.0f));
		ret &= RegisterUserInterface(m_matching_time_text);

		m_matching_cancel_button->SetNormalTexture("Contents/ui/cancel_normal.dds");
		m_matching_cancel_button->SetHoveredTexture("Contents/ui/cancel_hovered.dds");
		m_matching_cancel_button->SetPressedTexture("Contents/ui/cancel_pressed.dds");
		m_matching_cancel_button->SetPosition(window_size * 0.5f + Vec2(0.0f, 120.0f));
		m_matching_cancel_button->SetSize(Vec2(300.0f, 120.0f));
		m_matching_cancel_button->OnClicked([this]() {
			DisablePopUpState(eLobbyPopupMenu::kMatching);
			m_is_wait_matching = false;
			});
		DisablePopUpState(eLobbyPopupMenu::kMatching);
		ret &= RegisterUserInterface(m_matching_cancel_button);


		m_matching_success_image->SetTexture("Contents/ui/matching_success_menu.dds");
		m_matching_success_image->SetPosition(window_size * 0.5f);
		ret &= RegisterUserInterface(m_matching_success_image);

		m_game_start_button->SetNormalTexture("Contents/ui/game_start_normal.dds");
		m_game_start_button->SetHoveredTexture("Contents/ui/game_start_hovered.dds");
		m_game_start_button->SetPressedTexture("Contents/ui/game_start_pressed.dds");
		m_game_start_button->SetPosition(window_size * 0.5f + Vec2(0.0f, 120.0f));
		m_game_start_button->SetSize(Vec2(300.0f, 120.0f));
		m_game_start_button->OnClicked([this]() {
			DisablePopUpState(eLobbyPopupMenu::kGameStart);
			LevelManager::GetLevelManager().OpenLevel(CreateSPtr<GamePlayLevel>(), nullptr);
			});
		DisablePopUpState(eLobbyPopupMenu::kGameStart);
		ret &= RegisterUserInterface(m_game_start_button);

		return ret;
	}

	bool LobbyUILayer::GenerateOptionUI(const Vec2& window_size)
	{
		bool ret = true;

		m_close_app_button->SetNormalTexture("Contents/ui/exit_normal.dds");
		m_close_app_button->SetHoveredTexture("Contents/ui/exit_hovered.dds");
		m_close_app_button->SetPressedTexture("Contents/ui/exit_pressed.dds");
		m_close_app_button->SetPosition(Vec2(window_size.x - 60.0f, window_size.y) - Vec2(0.0f, 60.0f));
		m_close_app_button->SetSize(Vec2(70.0f, 70.0f));
		m_close_app_button->OnClicked([this]() {
			Application::GetApplication()->SetAppState(eAppState::kDead);
			});
		ret &= RegisterUserInterface(m_close_app_button);

		m_develop_mode_button->SetNormalTexture("Contents/ui/develop_normal.dds");
		m_develop_mode_button->SetHoveredTexture("Contents/ui/develop_hovered.dds");
		m_develop_mode_button->SetPressedTexture("Contents/ui/develop_pressed.dds");
		m_develop_mode_button->SetPosition(Vec2(60.0f, window_size.y) - Vec2(0.0f, 60.f));
		m_develop_mode_button->SetSize(Vec2(100.0f, 100.0f));
		m_develop_mode_button->SetActivate(false);
		m_develop_mode_button->SetVisible(false);
		m_develop_mode_button->OnClicked([this]() {
			LevelManager::GetLevelManager().OpenLevel(CreateSPtr<GamePlayLevel>(), nullptr);
			});
		ret &= RegisterUserInterface(m_develop_mode_button);

		m_option_button->SetNormalTexture("Contents/ui/option_normal.dds");
		m_option_button->SetHoveredTexture("Contents/ui/option_hovered.dds");
		m_option_button->SetPressedTexture("Contents/ui/option_pressed.dds");
		m_option_button->SetPosition(Vec2(window_size.x - 60.0f, window_size.y) - Vec2(0.0f, 140.0f));
		m_option_button->SetSize(Vec2(70.0f, 70.0f));
		m_option_button->OnClicked([this]() {
			EnablePopUpState(eLobbyPopupMenu::kOption);
			SetOptionState(eLobbyOptionState::kGraphic);
			});
		ret &= RegisterUserInterface(m_option_button);

		m_option_menu_image->SetTexture("Contents/ui/option_menu.dds");
		m_option_menu_image->SetPosition(window_size * 0.5f);
		ret &= RegisterUserInterface(m_option_menu_image);

		
		m_close_option_menu_button->SetNormalTexture("Contents/ui/close_normal.dds");
		m_close_option_menu_button->SetHoveredTexture("Contents/ui/close_hovered.dds");
		m_close_option_menu_button->SetPressedTexture("Contents/ui/close_pressed.dds");
		float button_size = m_option_menu_image->GetSize().x / 15.0f;
		m_close_option_menu_button->SetPosition(m_option_menu_image->GetPosition() +
			m_option_menu_image->GetSize() * Vec2(0.5f, -0.5f) + Vec2(-button_size, button_size));
		m_close_option_menu_button->SetSize(Vec2(button_size, button_size));
		m_close_option_menu_button->OnClicked([this]() {
			DisablePopUpState(eLobbyPopupMenu::kOption);
			});
		ret &= RegisterUserInterface(m_close_option_menu_button);
		
		m_option_graphic_button->SetNormalTexture("Contents/ui/graphic_normal.dds");
		m_option_graphic_button->SetHoveredTexture("Contents/ui/graphic_hovered.dds");
		m_option_graphic_button->SetPressedTexture("Contents/ui/graphic_pressed.dds");
		m_option_graphic_button->SetPosition(m_option_menu_image->GetPosition() +
			m_option_menu_image->GetSize() * Vec2(-0.16f, -0.40f) + Vec2(-248.f, 64.f)*0.7f);
		m_option_graphic_button->SetSize(Vec2(248.f, 64.f)*0.7f);
		m_option_graphic_button->OnClicked([this]() {
			SetOptionState(eLobbyOptionState::kGraphic);
		});
		ret &= RegisterUserInterface(m_option_graphic_button);

		m_option_sound_button->SetNormalTexture("Contents/ui/sound_normal.dds");
		m_option_sound_button->SetHoveredTexture("Contents/ui/sound_hovered.dds");
		m_option_sound_button->SetPressedTexture("Contents/ui/sound_pressed.dds");
		m_option_sound_button->SetPosition(m_option_menu_image->GetPosition() +
			m_option_menu_image->GetSize() * Vec2(0.05f, -0.40f) + Vec2(-248.f, 64.f) * 0.7f);
		m_option_sound_button->SetSize(Vec2(248.f, 64.f) * 0.7f);
		m_option_sound_button->OnClicked([this]() {
			SetOptionState(eLobbyOptionState::kSound);
		});
		ret &= RegisterUserInterface(m_option_sound_button);

		{
			auto SetLeftButton = [this, button_size, &ret](const SPtr<ButtonUI>& left_button, 
				const Vec2& position, std::function<void()>&& click_function)
			{
				left_button->SetNormalTexture("Contents/ui/left_normal.dds");
				left_button->SetHoveredTexture("Contents/ui/left_hovered.dds");
				left_button->SetPressedTexture("Contents/ui/left_pressed.dds");
				left_button->SetPosition(position);
				left_button->SetSize(Vec2(button_size, button_size));
				left_button->OnClicked(click_function);
				ret &= RegisterUserInterface(left_button);
			};

			auto SetRightButton = [this, button_size, &ret](const SPtr<ButtonUI>& left_button,
				const Vec2& position, std::function<void()>&& click_function)
			{
				left_button->SetNormalTexture("Contents/ui/right_normal.dds");
				left_button->SetHoveredTexture("Contents/ui/right_hovered.dds");
				left_button->SetPressedTexture("Contents/ui/right_pressed.dds");
				left_button->SetPosition(position);
				left_button->SetSize(Vec2(button_size, button_size));
				left_button->OnClicked(click_function);
				ret &= RegisterUserInterface(left_button);
			};

			auto SetOptionText = [this, &ret](const SPtr<TextUI> text,
				const Vec2& position)
			{
				text->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);
				text->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
				text->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
				text->SetFontName(L"배달의민족 주아");
				text->SetColor(Vec4(232.f, 137.f, 116.f, 255.0f) / 255.f);
				text->SetFontSize(34);
				text->SetPosition(position);
				ret &= RegisterUserInterface(text);
			};

			SetOptionText(m_graphic_option_shadow_enable_text,
				m_option_menu_image->GetPosition() + m_option_menu_image->GetSize() * Vec2(-0.16f, -0.32f) + Vec2(-button_size, button_size));

			SetOptionText(m_graphic_option_shadow_quality_text,
				m_option_menu_image->GetPosition() + m_option_menu_image->GetSize() * Vec2(-0.16f, -0.20f) + Vec2(-button_size, button_size));

			SetOptionText(m_sound_option_bgm_volume_text,
				m_option_menu_image->GetPosition() + m_option_menu_image->GetSize() * Vec2(-0.16f, -0.32f) + Vec2(-button_size, button_size));
			
			SetOptionText(m_sound_option_effect_volume_text,
				m_option_menu_image->GetPosition() + m_option_menu_image->GetSize() * Vec2(-0.16f, -0.20f) + Vec2(-button_size, button_size));

			SetLeftButton(m_shadow_enable_left_button, 
				m_option_menu_image->GetPosition() + m_option_menu_image->GetSize() * Vec2(0.12f, -0.32f) + Vec2(-button_size, button_size),
				[this]() {
					m_game_option_state->shadow_enable = false;
					SetShadowEnableText();
				});

			SetRightButton(m_shadow_enable_right_button,
				m_option_menu_image->GetPosition() + m_option_menu_image->GetSize() * Vec2(0.5f, -0.32f) + Vec2(-button_size, button_size),
				[this]() {
					m_game_option_state->shadow_enable = true;
					SetShadowEnableText();
				});

			SetOptionText(m_shadow_enable_text,
				m_option_menu_image->GetPosition() + m_option_menu_image->GetSize() * Vec2(0.3f, -0.32f) + Vec2(-button_size, button_size));

			SetLeftButton(m_shadow_quality_left_button,
				m_option_menu_image->GetPosition() + m_option_menu_image->GetSize() * Vec2(0.12f, -0.20f) + Vec2(-button_size, button_size),
				[this]() {
					switch (m_game_option_state->shadow_quality)
					{
					case eShadowQuality::kMiddle: m_game_option_state->shadow_quality = eShadowQuality::kLow; break;
					case eShadowQuality::kHigh: m_game_option_state->shadow_quality = eShadowQuality::kMiddle; break;
					case eShadowQuality::kVeryHigh: m_game_option_state->shadow_quality = eShadowQuality::kHigh; break;
					default: break;
					}
					SetShadowQualityText();
				});

			SetRightButton(m_shadow_quality_right_button,
				m_option_menu_image->GetPosition() + m_option_menu_image->GetSize() * Vec2(0.5f, -0.20f) + Vec2(-button_size, button_size),
				[this]() {
					switch (m_game_option_state->shadow_quality)
					{
					case eShadowQuality::kLow: m_game_option_state->shadow_quality = eShadowQuality::kMiddle; break;
					case eShadowQuality::kMiddle: m_game_option_state->shadow_quality = eShadowQuality::kHigh; break;
					case eShadowQuality::kHigh: m_game_option_state->shadow_quality = eShadowQuality::kVeryHigh; break;
					default: break;
					}
					SetShadowQualityText();
				});

			SetOptionText(m_shadow_quality_text,
				m_option_menu_image->GetPosition() + m_option_menu_image->GetSize() * Vec2(0.3f, -0.20f) + Vec2(-button_size, button_size));
			
			SetLeftButton(m_bgm_volume_left_button,
				m_option_menu_image->GetPosition() + m_option_menu_image->GetSize() * Vec2(0.12f, -0.32f) + Vec2(-button_size, button_size),
				[this]() {
				if (SoundManager::GetSoundManager().GetVolume() > 0.f)
				{
					SoundManager::GetSoundManager().VolumeDown(0.1f);
					SetBgmVolumeText(eSoundType::kBackGroundSound);
				}
			});

			SetRightButton(m_bgm_volume_right_button,
				m_option_menu_image->GetPosition() + m_option_menu_image->GetSize() * Vec2(0.5f, -0.32f) + Vec2(-button_size, button_size),
				[this]() {
				if (SoundManager::GetSoundManager().GetVolume() < 100.f)
				{
					SoundManager::GetSoundManager().VolumeUp(0.1f);
					SetBgmVolumeText(eSoundType::kBackGroundSound);
				}
			});

			SetOptionText(m_bgm_volume_text,
				m_option_menu_image->GetPosition() + m_option_menu_image->GetSize() * Vec2(0.3f, -0.32f) + Vec2(-button_size, button_size));
			
			SetLeftButton(m_effect_volume_left_button,
				m_option_menu_image->GetPosition() + m_option_menu_image->GetSize() * Vec2(0.12f, -0.20f) + Vec2(-button_size, button_size),
				[this]() {
				if (SoundManager::GetSoundManager().GetVolume(eSoundType::kEffectSound) > 0.f)
				{
					SoundManager::GetSoundManager().VolumeDown(0.1f, eSoundType::kEffectSound);
					SetBgmVolumeText(eSoundType::kEffectSound);
				}
			});

			SetRightButton(m_effect_volume_right_button,
				m_option_menu_image->GetPosition() + m_option_menu_image->GetSize() * Vec2(0.5f, -0.20f) + Vec2(-button_size, button_size),
				[this]() {
				if (SoundManager::GetSoundManager().GetVolume(eSoundType::kEffectSound) < 100.f)
				{
					SoundManager::GetSoundManager().VolumeUp(0.1f, eSoundType::kEffectSound);
					SetBgmVolumeText(eSoundType::kEffectSound);
				}
			});
			
			SetOptionText(m_effect_volume_text,
				m_option_menu_image->GetPosition() + m_option_menu_image->GetSize() * Vec2(0.3f, -0.20f) + Vec2(-button_size, button_size));
			
		
			SetBgmVolumeText(eSoundType::kBackGroundSound);
			SetBgmVolumeText(eSoundType::kEffectSound);
			SetShadowEnableText();
			SetShadowQualityText();
			DisablePopUpState(eLobbyPopupMenu::kOption);
		}
		return ret;
	}

	bool LobbyUILayer::GenrateSignUpUI(const Vec2& window_size)
	{
		bool ret = true;
		
		m_sign_up_succeed_image->SetTexture("Contents/ui/sign_up_success_menu.dds");
		m_sign_up_succeed_image->SetPosition(window_size * 0.5f);
		ret &= RegisterUserInterface(m_sign_up_succeed_image);

		m_sign_up_succeed_ok_button->SetNormalTexture("Contents/ui/ok_normal.dds");
		m_sign_up_succeed_ok_button->SetHoveredTexture("Contents/ui/ok_hovered.dds");
		m_sign_up_succeed_ok_button->SetPressedTexture("Contents/ui/ok_pressed.dds");
		m_sign_up_succeed_ok_button->SetPosition(window_size * 0.5f + Vec2(0.0f, 120.f));
		m_sign_up_succeed_ok_button->SetSize(Vec2(300.f, 120.f));
		m_sign_up_succeed_ok_button->OnClicked([this]() {
			DisablePopUpState(eLobbyPopupMenu::kSignUpSuccess);
		});
		DisablePopUpState(eLobbyPopupMenu::kSignUpSuccess);
		ret &= RegisterUserInterface(m_sign_up_succeed_ok_button);

		m_id_text->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);
		m_id_text->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_id_text->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		m_id_text->SetFontSize(20);
		m_id_text->SetPosition(window_size * 0.5f - Vec2(0.0f, 75.0f));
		m_id_text->SetColor(Vec4(0.f, 0.f, 0.f, 1.f));
		ret &= RegisterUserInterface(m_id_text);

		m_pw_text->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);
		m_pw_text->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_pw_text->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		m_pw_text->SetFontSize(20);
		m_pw_text->SetPosition(window_size * 0.5f - Vec2(0.0f, 30.0f));
		m_pw_text->SetColor(Vec4(0.f, 0.f, 0.f, 1.f));
		ret &= RegisterUserInterface(m_pw_text);

		return ret;
	}


	void LobbyUILayer::SetOptionState(eLobbyOptionState state)
	{
		auto SetUIVisiblity = [](const SPtr<UserInterface>& ui, bool value)
		{
			ui->SetActivate(value);
			ui->SetVisible(value);
		};

		switch (state)
		{
		case eLobbyOptionState::kGraphic:
			m_option_graphic_button->SetNormalTexture("Contents/ui/graphic_hovered.dds");
			m_option_sound_button->SetNormalTexture("Contents/ui/sound_normal.dds");
			SetUIVisiblity(m_graphic_option_shadow_enable_text, true);
			SetUIVisiblity(m_graphic_option_shadow_quality_text, true);
			SetUIVisiblity(m_shadow_quality_text, true);
			SetUIVisiblity(m_shadow_quality_left_button, true);
			SetUIVisiblity(m_shadow_quality_right_button, true);
			SetUIVisiblity(m_shadow_enable_text, true);
			SetUIVisiblity(m_shadow_enable_left_button, true);
			SetUIVisiblity(m_shadow_enable_right_button, true);
			SetUIVisiblity(m_bgm_volume_left_button, false);
			SetUIVisiblity(m_bgm_volume_right_button, false);
			SetUIVisiblity(m_bgm_volume_text, false);
			SetUIVisiblity(m_sound_option_bgm_volume_text, false);
			SetUIVisiblity(m_sound_option_effect_volume_text, false);
			SetUIVisiblity(m_effect_volume_text, false);
			SetUIVisiblity(m_effect_volume_left_button, false);
			SetUIVisiblity(m_effect_volume_right_button, false);
			break;
		case eLobbyOptionState::kSound:
			m_option_sound_button->SetNormalTexture("Contents/ui/sound_hovered.dds");
			m_option_graphic_button->SetNormalTexture("Contents/ui/graphic_normal.dds");
			SetUIVisiblity(m_graphic_option_shadow_enable_text, false);
			SetUIVisiblity(m_graphic_option_shadow_quality_text, false);
			SetUIVisiblity(m_shadow_quality_text, false);
			SetUIVisiblity(m_shadow_quality_left_button, false);
			SetUIVisiblity(m_shadow_quality_right_button, false);
			SetUIVisiblity(m_shadow_enable_text, false);
			SetUIVisiblity(m_shadow_enable_left_button, false);
			SetUIVisiblity(m_shadow_enable_right_button, false);
			SetUIVisiblity(m_bgm_volume_left_button, true);
			SetUIVisiblity(m_bgm_volume_right_button, true);
			SetUIVisiblity(m_bgm_volume_text, true);
			SetUIVisiblity(m_sound_option_bgm_volume_text, true);
			SetUIVisiblity(m_sound_option_effect_volume_text, true);
			SetUIVisiblity(m_effect_volume_text, true);
			SetUIVisiblity(m_effect_volume_left_button, true);
			SetUIVisiblity(m_effect_volume_right_button, true);
			break;
		}
	}

	void LobbyUILayer::SetPopUpState(eLobbyPopupMenu state, bool is_pop_up)
	{
		m_id_text_box->SetActivate(!is_pop_up);
		m_pw_text_box->SetActivate(!is_pop_up);
		m_sign_up_button->SetActivate(!is_pop_up);
		m_sign_in_button->SetActivate(!is_pop_up);
		m_matching_button->SetActivate(!is_pop_up);
		m_num_of_user_text->SetActivate(!is_pop_up);
		m_user_change_button->SetActivate(!is_pop_up);
		m_option_button->SetActivate(!is_pop_up);

		switch (state)
		{
		case eLobbyPopupMenu::kInvalidIDPW:
			m_invalid_id_pw_image->SetActivate(is_pop_up);
			m_invalid_id_pw_image->SetVisible(is_pop_up);
			m_invalid_id_pw_ok_button->SetActivate(is_pop_up);
			m_invalid_id_pw_ok_button->SetVisible(is_pop_up);
			break;
		case eLobbyPopupMenu::kAlreadyLogin:
			m_already_logged_in_image->SetActivate(is_pop_up);
			m_already_logged_in_image->SetVisible(is_pop_up);
			m_already_logged_in_ok_button->SetActivate(is_pop_up);
			m_already_logged_in_ok_button->SetVisible(is_pop_up);
			break;
		case eLobbyPopupMenu::kLoginSuccess:
			m_login_succeed_image->SetActivate(is_pop_up);
			m_login_succeed_image->SetVisible(is_pop_up);
			m_login_succeed_ok_button->SetActivate(is_pop_up);
			m_login_succeed_ok_button->SetVisible(is_pop_up);
			break;
		case eLobbyPopupMenu::kNotLogin:
			m_not_login_image->SetActivate(is_pop_up);
			m_not_login_image->SetVisible(is_pop_up);
			m_not_login_ok_button->SetActivate(is_pop_up);
			m_not_login_ok_button->SetVisible(is_pop_up);
			break;
		case eLobbyPopupMenu::kMatching:
			m_matching_image->SetActivate(is_pop_up);
			m_matching_image->SetVisible(is_pop_up);
			m_matching_time_text->SetActivate(is_pop_up);
			m_matching_time_text->SetVisible(is_pop_up);
			m_matching_cancel_button->SetActivate(is_pop_up);
			m_matching_cancel_button->SetVisible(is_pop_up);
			break;
		case eLobbyPopupMenu::kGameStart:
			m_game_start_button->SetActivate(is_pop_up);
			m_game_start_button->SetVisible(is_pop_up);
			m_matching_success_image->SetActivate(is_pop_up);
			m_matching_success_image->SetVisible(is_pop_up);
			break;
		case eLobbyPopupMenu::kOption:
			m_option_menu_image->SetActivate(is_pop_up);
			m_option_menu_image->SetVisible(is_pop_up);
			m_close_option_menu_button->SetActivate(is_pop_up);
			m_close_option_menu_button->SetVisible(is_pop_up);
			m_shadow_enable_left_button->SetActivate(is_pop_up);
			m_shadow_enable_left_button->SetVisible(is_pop_up);
			m_shadow_enable_right_button->SetActivate(is_pop_up);
			m_shadow_enable_right_button->SetVisible(is_pop_up);
			m_shadow_enable_text->SetActivate(is_pop_up);
			m_shadow_enable_text->SetVisible(is_pop_up);
			m_shadow_quality_left_button->SetVisible(is_pop_up);
			m_shadow_quality_left_button->SetActivate(is_pop_up);
			m_shadow_quality_right_button->SetVisible(is_pop_up);
			m_shadow_quality_right_button->SetActivate(is_pop_up);
			m_shadow_quality_text->SetActivate(is_pop_up);
			m_shadow_quality_text->SetVisible(is_pop_up);
			m_bgm_volume_left_button->SetActivate(is_pop_up);
			m_bgm_volume_left_button->SetVisible(is_pop_up);
			m_bgm_volume_right_button->SetActivate(is_pop_up);
			m_bgm_volume_right_button->SetVisible(is_pop_up);
			m_bgm_volume_text->SetActivate(is_pop_up);
			m_bgm_volume_text->SetVisible(is_pop_up);
			m_option_graphic_button->SetActivate(is_pop_up);
			m_option_graphic_button->SetVisible(is_pop_up);
			m_option_sound_button->SetActivate(is_pop_up);
			m_option_sound_button->SetVisible(is_pop_up);
			m_graphic_option_shadow_enable_text->SetActivate(is_pop_up);
			m_graphic_option_shadow_enable_text->SetVisible(is_pop_up);
			m_graphic_option_shadow_quality_text->SetActivate(is_pop_up);
			m_graphic_option_shadow_quality_text->SetVisible(is_pop_up);
			m_sound_option_bgm_volume_text->SetActivate(is_pop_up);
			m_sound_option_bgm_volume_text->SetVisible(is_pop_up);
			m_sound_option_effect_volume_text->SetActivate(is_pop_up);
			m_sound_option_effect_volume_text->SetVisible(is_pop_up);
			m_effect_volume_text->SetActivate(is_pop_up);
			m_effect_volume_text->SetVisible(is_pop_up);
			m_effect_volume_left_button->SetActivate(is_pop_up);
			m_effect_volume_left_button->SetVisible(is_pop_up);
			m_effect_volume_right_button->SetActivate(is_pop_up);
			m_effect_volume_right_button->SetVisible(is_pop_up);
			break;
		case eLobbyPopupMenu::kSignUpSuccess:
			m_sign_up_succeed_ok_button->SetActivate(is_pop_up);
			m_sign_up_succeed_ok_button->SetVisible(is_pop_up);
			m_sign_up_succeed_image->SetActivate(is_pop_up);
			m_sign_up_succeed_image->SetVisible(is_pop_up);
			m_id_text->SetActivate(is_pop_up);
			m_id_text->SetVisible(is_pop_up);
			m_pw_text->SetActivate(is_pop_up);
			m_pw_text->SetVisible(is_pop_up);
			break;
		default:
			break;
		}
	}

	void LobbyUILayer::EnablePopUpState(eLobbyPopupMenu state)
	{
		SetPopUpState(state, true);
	}

	void LobbyUILayer::DisablePopUpState(eLobbyPopupMenu state)
	{
		SetPopUpState(state, false);
	}

	void LobbyUILayer::SetShadowEnableText()
	{
		if (m_game_option_state->shadow_enable)
			m_shadow_enable_text->SetText(L"켜기");
		else
			m_shadow_enable_text->SetText(L"끄기");
	}

	void LobbyUILayer::SetShadowQualityText()
	{
		switch (m_game_option_state->shadow_quality)
		{
		case eShadowQuality::kLow: m_shadow_quality_text->SetText(L"저"); break;
		case eShadowQuality::kMiddle: m_shadow_quality_text->SetText(L"중"); 	break;
		case eShadowQuality::kHigh: m_shadow_quality_text->SetText(L"고"); break;
		case eShadowQuality::kVeryHigh: m_shadow_quality_text->SetText(L"최고"); 	break;
		default: break;
		}
	}

	void LobbyUILayer::SetBgmVolumeText(eSoundType sound_type)
	{
		float volume = SoundManager::GetSoundManager().GetVolume(sound_type);

		if(eSoundType::kBackGroundSound == sound_type)
			m_bgm_volume_text->SetText(std::to_wstring(static_cast<int>(round(volume * 100)) ));
		else if(eSoundType::kEffectSound == sound_type)
			m_effect_volume_text->SetText(std::to_wstring(static_cast<int>(round(volume * 100))));
	}

	void LobbyUILayer::FailedLogin(eLoginFailState state)
	{
		switch (state)
		{
		case eLoginFailState::kAlreadyLogin:
			EnablePopUpState(eLobbyPopupMenu::kAlreadyLogin);
			break;
		case eLoginFailState::kInvalidIDPW:
			EnablePopUpState(eLobbyPopupMenu::kInvalidIDPW);
			break;
		default:
			break;
		}
	}

	void LobbyUILayer::SucceededLogin()
	{
		EnablePopUpState(eLobbyPopupMenu::kLoginSuccess);
		m_succeeded_login = true;
	}

	void LobbyUILayer::SucceededMatching()
	{
		EnablePopUpState(eLobbyPopupMenu::kGameStart);
		m_is_wait_matching = false;
	}

	void LobbyUILayer::SucceededSignUp()
	{
		EnablePopUpState(eLobbyPopupMenu::kSignUpSuccess);
		m_id_text->SetText(m_id);
		m_pw_text->SetText(m_pw);
	}

	void LobbyUILayer::EnableDevelopMode()
	{
		m_develop_mode_button->SetActivate(true);
		m_develop_mode_button->SetVisible(true);
	}
}
