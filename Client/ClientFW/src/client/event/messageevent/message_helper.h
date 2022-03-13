#pragma once

namespace client_fw
{
	class Actor;
	class Level;
	class MessageEventInfo;

	class MessageHelper final
	{
	public:
		static void RegisterMessageEvent(SPtr<MessageEventInfo>&& message);

		static void RegisterMessageReceiver(UINT event_id, const SPtr<Actor>& actor);
		static void UnregisterMessageReceiver(UINT event_id, const SPtr<Actor>& actor);
	};
}



