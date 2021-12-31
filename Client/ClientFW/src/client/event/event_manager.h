#pragma once

namespace client_fw
{
	class EventManager
	{
	public:
		EventManager() {}
		virtual ~EventManager() = default;

		EventManager(const EventManager&) = delete;
		EventManager& operator=(const EventManager&) = delete;

		virtual void ExecuteEvent() = 0;
	};
}

