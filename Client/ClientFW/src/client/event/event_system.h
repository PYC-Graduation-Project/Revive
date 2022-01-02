#pragma once

namespace client_fw
{
	class IEventSystem
	{
	public:
		IEventSystem() {};
		virtual ~IEventSystem() = default;

		virtual void ExecuteEvent() = 0;
	};
}



