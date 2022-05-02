#pragma once
#include <client/object/ui/core/user_interface_layer.h>

namespace client_fw
{
	class ImageUI;
	class ButtonUI;
	class TextBoxUI;
	class TextUI;
}

namespace revive
{
	using namespace client_fw;

	constexpr static UINT s_min_num_of_user = 2;
	constexpr static UINT s_max_num_of_user = 3;

	enum class eLobbyPopupMenu
	{
		kInvalidIDPW, kAlreadyLogin, kLoginSuccess,
		kNotLogin, kMatching, kGameStart
	};

	enum class eLoginFailState
	{
		kAlreadyLogin, kInvalidIDPW
	};

	class LobbyUILayer : public UserInterfaceLayer
	{
	public:
		LobbyUILayer();
		virtual ~LobbyUILayer() = default;

		virtual bool Initialize() override;
		virtual void Update(float delta_time) override;

	private:
		bool GenerateMatchingUI(const Vec2& window_size);
		bool GenerateSignInMenuUI(const Vec2& window_size);
		bool GenerateMatchingMenuUI(const Vec2& window_size);
		bool GenerateOptionUI(const Vec2& window_size);

	private:
		SPtr<TextBoxUI> m_id_text_box;
		SPtr<TextBoxUI> m_pw_text_box;
		SPtr<ButtonUI> m_sign_up_button;
		SPtr<ButtonUI> m_sign_in_button;
		SPtr<ButtonUI> m_matching_button;
		SPtr<TextUI> m_num_of_user_text;
		SPtr<ButtonUI> m_user_change_button;

		SPtr<ImageUI> m_invalid_id_pw_image;
		SPtr<ButtonUI> m_invalid_id_pw_ok_button;
		SPtr<ImageUI> m_already_logged_in_image;
		SPtr<ButtonUI> m_already_logged_in_ok_button;
		SPtr<ImageUI> m_login_succeed_image;
		SPtr<ButtonUI> m_login_succeed_ok_button;

		SPtr<ImageUI> m_not_login_image;
		SPtr<ButtonUI> m_not_login_ok_button;
		SPtr<ImageUI> m_matching_image;
		SPtr<TextUI> m_matching_time_text;
		SPtr<ButtonUI> m_matching_cancel_button;
		SPtr<ImageUI> m_matching_success_image;
		SPtr<ButtonUI> m_game_start_button;

		SPtr<ButtonUI> m_close_app_button;
		SPtr<ButtonUI> m_develop_mode_button;

	private:
		std::string m_id;
		std::string m_pw;
		bool m_succeeded_login = false;
		UINT m_num_of_user = 2;

		bool m_is_wait_matching = false;
		float m_matching_time = 0.0f;
		
	private:
		void SetPopUpState(eLobbyPopupMenu state, bool value);
		void EnablePopUpState(eLobbyPopupMenu state);
		void DisablePopUpState(eLobbyPopupMenu state);

	public:
		void FailedLogin(eLoginFailState state);
		void SucceededLogin();
		void SucceededMatching();
		void EnableDevelopMode();
	};
}


