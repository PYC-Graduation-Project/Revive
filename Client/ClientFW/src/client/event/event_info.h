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
}



