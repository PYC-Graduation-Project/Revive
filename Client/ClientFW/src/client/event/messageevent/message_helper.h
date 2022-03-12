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
		static void RegisterMessageReceiver(const std::string& message_name, const SPtr<Actor>& actor);
		static void RegisterMessageReceiver(const std::string& message_name, const SPtr<Level>& level);
		static void UnregisterMessageReceiver(const std::string& message_name, const SPtr<Actor>& actor);
		static void UnregisterMessageReceiver(const std::string& message_name, const SPtr<Level>& level);
	};
}



