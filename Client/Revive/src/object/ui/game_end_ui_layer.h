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

	enum class eGameState
	{
		kWin,
		kDefeat,
		kRun
	};

	class GameEndUILayer : public UserInterfaceLayer
	{
	public:
		GameEndUILayer();
		virtual ~GameEndUILayer() = default;

		virtual bool Initialize() override;
		virtual void Update(float delta_time) override;

	private:
		eGameState m_game_state;
		SPtr<ImageUI> m_end_game_image;
		SPtr<ButtonUI> m_to_lobby_button;

		float m_lobby_button_enable_time = 3.0f;

	public:
		eGameState GetGameState() const { return m_game_state; }
		void SetGameState(eGameState game_state) { m_game_state = game_state; }
	};
}



