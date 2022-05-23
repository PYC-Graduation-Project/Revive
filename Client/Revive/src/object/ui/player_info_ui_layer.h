#pragma once
#include <client/object/ui/core/user_interface_layer.h>

namespace client_fw
{
    class TextUI;
    class ImageUI;
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
        //SPtr<ProgressBarUI> hp_bar;
        std::array<SPtr<ImageUI>, 3> hp_bar_bg;
        std::array<SPtr<ImageUI>, 3> hp_bar_fg;
        SPtr<TextUI> hp_text;
        WPtr<DefaultPlayer> other_player;
    };

    struct WaveUIInfo
    {
        SPtr<ImageUI> wave_info_bg_image;
        SPtr<ImageUI> wave_info_fg_image;
        SPtr<ImageUI> wave_info_icon_image;
        SPtr<TextUI> wave_info_text;
        std::string bg_image_path;
        std::string fg_image_path;
        std::string icon_image_path;
        std::function<void(float)> function;
    };

    struct HPBarResourceInfo
    {
        std::string shape;
        Vec2 position;
        Vec2 size;
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
        int m_wave_count = 1;
        int m_mob_count = 0;
        float m_next_wave_time = 30.f;

        std::vector<WaveUIInfo> m_wave_ui_info;

        //SPtr<ProgressBarUI> m_hp_bar;
        std::array<SPtr<ImageUI>,3> m_hp_bar_bg;
        std::array<SPtr<ImageUI>,3> m_hp_bar_fg;
        SPtr<TextUI> m_hp_text;
        WPtr<RevivePlayer> m_revive_player;
        
        SPtr<ImageUI> m_crosshair_image;

        
        

    private:
        std::vector<OtherPlayerUIInfo> m_other_players_info;

    public:
        void SetOwnerPlayer(const WPtr<RevivePlayer>& player); 
        void RegisterOtherPlayer(const WPtr<DefaultPlayer>& player);
    };



}

