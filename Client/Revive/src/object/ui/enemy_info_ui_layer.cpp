#include <include/client_core.h>
#include <client/object/ui/text_ui.h>
#include <client/object/ui/progress_bar_ui.h>

#include "enemy_info_ui_layer.h"
#include "object/actor/character/enemy.h"

namespace revive
{
	EnemyInfoUILayer::EnemyInfoUILayer()
		: UserInterfaceLayer("enemy info ui layer")
	{
#ifdef __USE_ENEMY_ATTACK_PACKET_DEBUG__
		m_text = CreateSPtr<TextUI>("Test Text", Vec2(200.0f, 60.f), L"ID : 0\n 회전 속도 : 180");
#endif
		m_hp_bar = CreateSPtr<ProgressBarUI>("enemy hp bar");
	}

	bool EnemyInfoUILayer::Initialize()
	{
#ifdef __USE_ENEMY_ATTACK_PACKET_DEBUG__
		m_text->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);
		m_text->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_text->SetFontSize(20);
		m_text->SetPosition(Vec2(0.0f, 40.0f));
		RegisterUserInterface(m_text);
		
		const auto& owner = m_enemy.lock();
		if (owner != nullptr)
		{
			std::time_t timer = time(nullptr);
			struct tm* t = localtime(&timer);

			m_text->SetText(L"ID : " + std::to_wstring(owner->GetNetworkID()) + L"\n Time : " +
				std::to_wstring(t->tm_hour) + L":" + std::to_wstring(t->tm_min) + L":" +
				std::to_wstring(t->tm_sec));

			owner->OnAttackPacketStateFunction([this](int id)
				{
					std::time_t timer = time(nullptr);
					struct tm* t = localtime(&timer);

					m_text->SetText(L"ID : " + std::to_wstring(id) + L"\n Time : " +
						std::to_wstring(t->tm_hour) + L":" + std::to_wstring(t->tm_min) + L":" +
						std::to_wstring(t->tm_sec));
				});
		}
#endif

		m_hp_bar->SetBackgroundTexture("../Contents/hp_background.dds");
		m_hp_bar->SetFillTexture("../Contents/hp_bar.dds");
		m_hp_bar->SetSize(Vec2(150.f, 24.f));
		m_hp_bar->SetPosition(Vec2(0.0f, 0.0f));
		m_hp_bar->SetPivot(Vec2(0.0f, 0.0f));
		m_hp_bar->SetPercent(1.0f);
		RegisterUserInterface(m_hp_bar);

		return true;
	}

	void EnemyInfoUILayer::Update(float delta_time)
	{
	}

	void EnemyInfoUILayer::SetHPPercent(float percent)
	{
		m_hp_bar->SetPercent(percent);
	}
}