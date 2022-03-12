#include "stdafx.h"
#include "client/event/messageevent/message_helper.h"
#include "client/event/messageevent/message_event_manager.h"
#include "client/event/core/event_system.h"

namespace client_fw
{
	void MessageHelper::RegisterMessageEvent(SPtr<MessageEventInfo>&& message)
	{
		EventSystem::GetEventSystem().GetMessageEventManager()->RegisterMessageEvent(std::move(message));
	}

	void MessageHelper::RegisterMessageReceiver(const std::string& message_name, const SPtr<Actor>& actor)
	{
		EventSystem::GetEventSystem().GetMessageEventManager()->RegisterMessageReceiver(message_name, actor);
	}

	void MessageHelper::RegisterMessageReceiver(const std::string& message_name, const SPtr<Level>& level)
	{
		EventSystem::GetEventSystem().GetMessageEventManager()->RegisterMessageReceiver(message_name, level);
	}

	void MessageHelper::UnregisterMessageReceiver(const std::string& message_name, const SPtr<Actor>& actor)
	{
		EventSystem::GetEventSystem().GetMessageEventManager()->UnregisterMessageReceiver(message_name, actor);
	}

	void MessageHelper::UnregisterMessageReceiver(const std::string& message_name, const SPtr<Level>& level)
	{
		EventSystem::GetEventSystem().GetMessageEventManager()->UnregisterMessageReceiver(message_name, level);
	}
}
