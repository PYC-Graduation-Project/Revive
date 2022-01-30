#pragma once
#include "client/event/event_manager.h"

namespace client_fw
{
	class InputEventInfo;
	enum class eInputMode;

	class InputEventManager final : public IEventManager
	{
	public:
		InputEventManager();
		virtual ~InputEventManager();

		InputEventManager(const InputEventManager&) = delete;
		InputEventManager& operator=(const InputEventManager&) = delete;

		virtual void ExecuteEvent() override;

	private:
		void ExecuteEvents(std::vector<UPtr<InputEventInfo>>& events, eInputMode mode);
		void DeleteEvent(std::vector<UPtr<InputEventInfo>>& events, const std::string& name);

	public:	
		bool RegisterEvent(UPtr<InputEventInfo>&& event_info, eInputOwnerType type);
		void UnregisterEvent(const std::string& name);
		eInputMode GetInputMode() const { return m_input_mode; }
		void SetInputMode(eInputMode mode) { m_input_mode = mode; }

	private:
		eInputMode m_input_mode;
		std::map<std::string, eInputOwnerType> m_event_names;

		std::vector<UPtr<InputEventInfo>> m_application_events;
		std::vector<UPtr<InputEventInfo>> m_level_events;
		std::vector<UPtr<InputEventInfo>> m_actor_events;
		std::vector<UPtr<InputEventInfo>> m_pawn_events;
	};
}