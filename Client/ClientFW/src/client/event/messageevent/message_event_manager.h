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

		void RegisterMessageReceiver(UINT event_id, const SPtr<Actor>& actor);
		void UnregisterMessageReceiver(UINT event_id, const SPtr<Actor>& actor);

	private:
		std::queue<SPtr<MessageEventInfo>> m_message_queue;

		std::multimap<UINT, SPtr<Actor>> m_message_receive_actors;
	};
}