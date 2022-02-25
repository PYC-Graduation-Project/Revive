#include "stdafx.h"
#include "client/event/uievent/ui_event_manager.h"
#include "client/event/uievent/ui_evevt_info.h"
#include "client/object/ui/core/user_interface.h"

namespace client_fw
{
	UIEventManager::UIEventManager()
	{
	}

	UIEventManager::~UIEventManager()
	{
	}

	void UIEventManager::ExecuteEvent()
	{
		UINT dead_count = 0;

		for (auto ui_iter = m_ui_events.rbegin(); ui_iter != m_ui_events.rend(); ++ui_iter)
		{
			const auto& ui = *ui_iter;

			if (ui->GetOwnerUI() != nullptr)
			{
				ui->ExecuteEvent();
			}
			else
			{
				std::iter_swap(ui_iter, m_ui_events.rbegin() + dead_count);
				++dead_count;
			}
		}

		while (dead_count--)
			m_ui_events.pop_back();
	}

	void UIEventManager::RegisterEvent(UPtr<UIEventInfo>&& event_info)
	{
		m_ui_events.emplace_back(std::move(event_info));
	}
}
