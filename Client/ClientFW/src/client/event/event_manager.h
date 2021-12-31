#pragma once

namespace client_fw
{
	class EventInfo
	{
	public:
		EventInfo() {}
		virtual ~EventInfo() = default;

		virtual void ExecuteEvent() const = 0;
	};

	class EventManager
	{
	public:
		EventManager() {}

		EventManager(const EventManager&) = delete;
		EventManager& operator=(const EventManager&) = delete;

		virtual void ExecuteEvent() = 0;
	};
}

