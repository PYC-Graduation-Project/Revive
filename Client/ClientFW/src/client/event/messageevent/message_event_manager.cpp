#include "stdafx.h"
#include "client/event/messageevent/message_event_manager.h"
#include "client/event/messageevent/message_event_info.h"
#include "client/object/actor/core/actor.h"
#include "client/object/level/core/level.h"

namespace client_fw
{
	MessageEventManager::MessageEventManager()
	{
	}

	MessageEventManager::~MessageEventManager()
	{
	}

	void MessageEventManager::ExecuteEvent()
	{
		while (m_message_queue.empty() == false)
		{
			const auto& message = m_message_queue.front();
			const auto& message_name = message->GetName();

			for (auto iter = m_message_receive_actors.lower_bound(message_name);
				iter != m_message_receive_actors.upper_bound(message_name); ++iter)
			{
				iter->second->ExecuteMessage(message);
			}

			for (auto iter = m_message_receive_levels.lower_bound(message_name);
				iter != m_message_receive_levels.upper_bound(message_name); ++iter)
			{
				iter->second->ExecuteMessage(message);
			}

			m_message_queue.pop();
		}
	}

	void MessageEventManager::RegisterMessageEvent(SPtr<MessageEventInfo>&& message)
	{
		m_message_queue.emplace(std::move(message));
	}

	void MessageEventManager::RegisterMessageReceiver(const std::string& message_name, const SPtr<Actor>& actor)
	{
		m_message_receive_actors.emplace(message_name, actor);
	}

	void MessageEventManager::RegisterMessageReceiver(const std::string& message_name, const SPtr<Level>& level)
	{
		m_message_receive_levels.emplace(message_name, level);
	}

	void MessageEventManager::UnregisterMessageReceiver(const std::string& message_name, const SPtr<Actor>& actor)
	{
		for (auto iter = m_message_receive_actors.lower_bound(message_name);
			iter != m_message_receive_actors.upper_bound(message_name); ++iter)
		{
			if (iter->second == actor)
			{
				iter = m_message_receive_actors.erase(iter);
				break;
			}
		}
	}

	void MessageEventManager::UnregisterMessageReceiver(const std::string& message_name, const SPtr<Level>& level)
	{
		for (auto iter = m_message_receive_levels.lower_bound(message_name);
			iter != m_message_receive_levels.upper_bound(message_name); ++iter)
		{
			if (iter->second == level)
			{
				iter = m_message_receive_levels.erase(iter);
				break;
			}
		}
	}
}
