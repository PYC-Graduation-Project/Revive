#include <include/client_core.h>
#include <client/object/ui/text_ui.h>

#include "enemy_info_ui_layer.h"
#include "object/actor/character/enemy.h"

namespace revive
{
	EnemyInfoUILayer::EnemyInfoUILayer()
		: UserInterfaceLayer("enemy info ui layer")
	{
	}

	bool EnemyInfoUILayer::Initialize()
	{
		m_text = CreateSPtr<TextUI>("Test Text", Vec2(200.0f, 60.f), L"ID : 0\n 회전 속도 : 180");
		RegisterUserInterface(m_text);
		m_text->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);
		m_text->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_text->SetFontSize(20);
		
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

		return true;
	}

	void EnemyInfoUILayer::Update(float delta_time)
	{
	}
}