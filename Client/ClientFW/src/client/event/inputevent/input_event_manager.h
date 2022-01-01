#pragma once
#include "client/event/event_manager.h"

namespace client_fw
{
	class InputEventInfo;

	class InputEventManager final : public EventManager
	{
	public:
		InputEventManager();
		virtual ~InputEventManager();

		InputEventManager(const InputEventManager&) = delete;
		InputEventManager& operator=(const InputEventManager&) = delete;

		virtual void ExecuteEvent() override;

	public:
		void RegisterEvent(UPtr<InputEventInfo>&& event_info, eInputOwnerType type);

	private:
		std::set<std::string_view> m_event_names;
		std::vector<UPtr<InputEventInfo>> m_application_events;
	};
}