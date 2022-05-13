#pragma once
#include <client/object/ui/core/user_interface_layer.h>

namespace client_fw
{
    class TextUI;
    class ProgressBarUI;
}

namespace revive
{
    using namespace client_fw;

    class DefaultPlayer;
    class RevivePlayer;

    struct OtherPlayerUIInfo
    {
        SPtr<TextUI> id_text;
        SPtr<ProgressBarUI> hp_bar;
        SPtr<TextUI> hp_text;
        WPtr<DefaultPlayer> other_player;
    };

    class PlayerInfoUILayer : public UserInterfaceLayer
    {
    public:
        PlayerInfoUILayer();
        virtual ~PlayerInfoUILayer() = default;

        virtual bool Initialize() override;
        virtual void Update(float delta_time) override;

    private:
        bool GeneratePlayerUI(const Vec2& window_size);

    private:
        SPtr<ProgressBarUI> m_hp_bar;
        SPtr<TextUI> m_hp_text;

        WPtr<RevivePlayer> m_revive_player;

    private:
        std::vector<OtherPlayerUIInfo> m_other_players_info;

    public:
        void SetOwnerPlayer(const WPtr<RevivePlayer>& player); 
        void RegisterOtherPlayer(const WPtr<DefaultPlayer>& player);
    };



}

