#pragma once

namespace client_fw
{
	struct EventKeyInfo;
	struct AxisEventKeyInfo;

	struct InputEventInfo
	{
		std::string_view event_name;
		std::vector<EventKeyInfo> event_keys;
		bool comsume_input;
		std::function<void()> event_func;
	};
	
	struct AxisInputEventInfo
	{
		std::string_view event_name;
		std::vector<AxisEventKeyInfo> event_keys;
		bool comsume_input;
		std::function<void(float)> event_func;
	};

	class InputEventManager final
	{
	private:
		
		enum class EInputEventType
		{
			kPressed, kReleased, kAxis
		};

	public:
		InputEventManager();

		void ProcessInput();

		void RegisterPressedEvent(std::string_view name, std::vector<EventKeyInfo>&& keys,
			const std::function<void()>& func, bool consumption);
		/*void RegisterReleasedEvent(std::string_view name, std::vector<EventKeyInfo>&& keys,
			const std::function<void()>& func, bool consumption = true);
		void RegisterAxisEvent(std::string_view name, std::vector<AxisEventKeyInfo>&& keys,
			const std::function<void()>& func, bool consumption = true);

		void ChangeInputEventKey(std::string_view name, std::vector<EventKeyInfo>&& keys);
		void ChangeAxisInputEventKey(std::string_view name, std::vector<AxisEventKeyInfo>&& keys);*/

	private:
		std::map<std::string, EInputEventType> m_event_names;

		std::vector<InputEventInfo> m_pressed_events;
		std::vector<InputEventInfo> m_released_events;
		std::vector<AxisInputEventInfo> m_axis_events;
	};
}