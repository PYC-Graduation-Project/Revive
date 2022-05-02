#pragma once
#include <client/object/ui/core/user_interface_layer.h>

namespace client_fw
{
	class ImageUI;
	class ButtonUI;
}

namespace revive
{
	using namespace client_fw;

	enum class eGameResult
	{
		kWin,
		kDefeat
	};

	class GameEndUILayer : public UserInterfaceLayer
	{
	public:
		GameEndUILayer(eGameResult game_result);
		virtual ~GameEndUILayer() = default;

		virtual bool Initialize() override;
		virtual void Update(float delta_time) override;

	private:
		eGameResult m_game_result;
		SPtr<ImageUI> m_end_game_image;
		SPtr<ButtonUI> m_to_lobby_button;

		float m_lobby_button_enable_time = 3.0f;
	};
}



