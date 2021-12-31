#pragma once

namespace client_fw
{
	class EventSystem
	{
	public:
		EventSystem() {};
		virtual ~EventSystem() = default;

		virtual void ExecuteEvent() = 0;
	};
}



