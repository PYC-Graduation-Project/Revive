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

    class Enemy;

    class EnemyInfoUILayer : public UserInterfaceLayer
    {
    public:
        EnemyInfoUILayer();
        virtual ~EnemyInfoUILayer() = default;

        virtual bool Initialize() override;
        virtual void Update(float delta_time) override;

    private:
        SPtr<TextUI> m_text;
        SPtr<ProgressBarUI> m_hp_bar;
        WPtr<Enemy> m_enemy;

    public:
        void SetHPPercent(float percent);

        void SetEnemy(const WPtr<Enemy>& owner) { m_enemy = owner; }
    };
}



