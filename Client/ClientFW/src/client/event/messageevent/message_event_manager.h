#pragma once
#include "client/event/core/event_manager.h"

namespace client_fw
{
	class MessageEventInfo;
	class Actor;
	class Level;

	class MessageEventManager final : public IEventManager
	{
	public:
		MessageEventManager();
		virtual ~MessageEventManager();

		MessageEventManager(const MessageEventManager&) = delete;
		MessageEventManager& operator=(const MessageEventManager&) = delete;

		virtual void ExecuteEvent() override;

	public:
		void RegisterMessageEvent(SPtr<MessageEventInfo>&& message);
		void RegisterMessageReceiver(const std::string& message_name, const SPtr<Actor>& actor);
		void RegisterMessageReceiver(const std::string& message_name, const SPtr<Level>& level);
		void UnregisterMessageReceiver(const std::string& message_name, const SPtr<Actor>& actor);
		void UnregisterMessageReceiver(const std::string& message_name, const SPtr<Level>& level);

	private:
		std::queue<SPtr<MessageEventInfo>> m_message_queue;

		std::multimap<std::string, SPtr<Actor>> m_message_receive_actors;
		std::multimap<std::string, SPtr<Level>> m_message_receive_levels;

	};
}