#pragma once
#include "client/event/event_info.h"

namespace client_fw
{
	struct EventKeyInfo;
	struct AxisEventKeyInfo;

	class InputEventInfo : public IEventInfo
	{
	public:
		InputEventInfo(std::string_view event_name, bool is_comsume);
		virtual ~InputEventInfo() = default;

	protected:
		std::string_view m_event_name;
		bool m_is_comsume_input;

	public:
		std::string_view GetEventName() const { return m_event_name; }
	};

	class ActionEventInfo : public InputEventInfo
	{
	public:
		ActionEventInfo(std::string_view event_name, bool is_comsume,
			std::vector<EventKeyInfo>&& event_keys, const std::function<void()>& event_func);
		virtual ~ActionEventInfo() = default;

	protected:
		std::vector<EventKeyInfo> m_event_keys;
		std::function<void()> m_event_func;
	};

	class PressedEventInfo final : public ActionEventInfo
	{
	public:
		PressedEventInfo(std::string_view event_name, bool is_comsume,
			std::vector<EventKeyInfo>&& event_keys, const std::function<void()>& event_func);
		virtual ~PressedEventInfo() = default;

		virtual void ExecuteEvent() const override;
	};

	class ReleasedEventInfo final : public ActionEventInfo
	{
	public:
		ReleasedEventInfo(std::string_view event_name, bool is_comsume,
			std::vector<EventKeyInfo>&& event_keys, const std::function<void()>& event_func);
		virtual ~ReleasedEventInfo() = default;

		virtual void ExecuteEvent() const override;
	};

	class AxisEventInfo final : public InputEventInfo
	{
	public:
		AxisEventInfo(std::string_view event_name, bool is_comsume,
			std::vector<AxisEventKeyInfo>&& event_keys, const std::function<void(float)>& event_func);
		virtual ~AxisEventInfo() = default;

		virtual void ExecuteEvent() const override;

	protected:
		std::vector<AxisEventKeyInfo> m_event_keys;
		std::function<void(float)> m_event_func;
	};
}
