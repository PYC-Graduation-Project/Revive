#pragma once
#include <client/object/ui/core/user_interface_layer.h>

namespace client_fw
{
    class TextUI;
    class ImageUI;
}

namespace revive
{
    using namespace client_fw;    
    
    class Enemy;
    class Base;
   // struct HPBarResourceInfo;
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

    class GameInfoUILayer : public UserInterfaceLayer
    {
    public:
        GameInfoUILayer();
        virtual ~GameInfoUILayer() = default;

        virtual bool Initialize() override;
        virtual void Update(float delta_time) override;

    private:
        bool GenerateGameInfoUI(const Vec2& window_size);
        bool GenerateNextWaveInfoUI(const Vec2& window_size);

    private:

        bool m_is_visible_next_wave_info = false;

        int m_wave_count = 0;
        int m_mob_count = 0;
        float m_next_wave_time = 30.f;
        float m_next_wave_info_visible_time = 0.f;
        std::vector<WaveUIInfo> m_wave_ui_info;

        SPtr<ImageUI> m_next_wave_bg_image;
        SPtr<TextUI> m_skeleton_king_count_text;
        SPtr<TextUI> m_skeleton_soldier_count_text;
        SPtr<ImageUI> m_base_icon_image;
        std::array<SPtr<ImageUI>, 3>  m_base_hp_bar_bg_image;
        std::array<SPtr<ImageUI>, 3>  m_base_hp_bar_fg_image;

    public:
        void SetWaveTime(float time) { m_next_wave_time = time; }
        void SetNextWaveCount(const int soldier_num, const int king_num);
        void SetVisibleNextWaveInfo(bool value);
        void RegisterEnemy(const WPtr<Enemy>& enemy);
        void RegisterBase(const WPtr<Base>& base);
    };
}