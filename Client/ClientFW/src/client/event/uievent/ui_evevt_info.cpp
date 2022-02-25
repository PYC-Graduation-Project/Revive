#include "stdafx.h"
#include "client/event/uievent/ui_evevt_info.h"

namespace client_fw
{
	UIEventInfo::UIEventInfo(const WPtr<UserInterface>& owner, 
		const std::function<void()>& event_func)
		: m_onwer_ui(owner), m_event_func(event_func)
	{
	}

	void UIEventInfo::ExecuteEvent() const
	{
		m_event_func();
	}
}
