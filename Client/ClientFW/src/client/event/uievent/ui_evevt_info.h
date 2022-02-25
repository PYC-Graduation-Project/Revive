#pragma once
#include "client/event/core/event_info.h"

namespace client_fw
{
	class UserInterface;

	class UIEventInfo : public IEventInfo
	{
	public:
		UIEventInfo(const WPtr<UserInterface>& owner,
			const std::function<void()>& event_func);

		virtual void ExecuteEvent() const override;

	protected:
		WPtr<UserInterface> m_onwer_ui;
		std::function<void()> m_event_func;

	public:
		SPtr<UserInterface> GetOwnerUI() const { return m_onwer_ui.lock(); }
	};
}


