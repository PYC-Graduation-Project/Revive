#include "stdafx.h"
#include "client/object/ui/core/user_interface_manager.h"
#include "client/object/ui/core/user_interface.h"

namespace client_fw
{
	UserInterfaceManager* UserInterfaceManager::s_ui_manager = nullptr;

	UserInterfaceManager::UserInterfaceManager()
	{
		s_ui_manager = this;
	}

	void UserInterfaceManager::Shutdown()
	{
		for (const auto& ui : m_user_interfaces)
			ui->Shutdown();
	}

	void UserInterfaceManager::Update(float delta_time)
	{
		int dead_count = 0;

		for (auto ui = m_user_interfaces.rbegin(); ui != m_user_interfaces.rend(); ++ui)
		{
			switch ((*ui)->GetUIState())
			{
			case eUIState::kActive:
				(*ui)->Update(delta_time);
				break;
			case eUIState::kDead:
				(*ui)->Shutdown();
				std::iter_swap(ui, m_user_interfaces.rbegin() + dead_count);
				++dead_count;
				break;
			default:
				break;
			}
		}

		while (dead_count--)
			m_user_interfaces.pop_back();
	}
	bool UserInterfaceManager::RegisterUserInterface(const SPtr<UserInterface>& ui)
	{
		if (ui->Initialize())
		{
			m_user_interfaces.push_back(ui);
			return true;
		}
		else
		{
			LOG_ERROR("Could not initialize user interface : {0}", ui->GetName());
			ui->Shutdown();
			return false;
		}
	}
}
