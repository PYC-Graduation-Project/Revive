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
		s_ui_manager = nullptr;
	}

	void UserInterfaceManager::Reset()
	{
		for (const auto& ui : m_user_interfaces)
			ui->SetUIState(eUIState::kDead);
	}

	void UserInterfaceManager::Update(float delta_time)
	{
		for (auto ui = m_user_interfaces.cbegin(); ui != m_user_interfaces.cend();)
		{
			switch ((*ui)->GetUIState())
			{
			case eUIState::kActive:
				(*ui)->Update(delta_time);
				++ui;
				break;
			case eUIState::kHide:
				++ui;
				break;
			case eUIState::kDead:
				m_num_of_visible_texture -= (*ui)->GetNumOfVisibleTexture();
				(*ui)->Shutdown();
				ui = m_user_interfaces.erase(ui);
				break;
			default:
				break;
			}
		}
	}

	bool UserInterfaceManager::RegisterUserInterface(const SPtr<UserInterface>& ui)
	{
		if (ui->Initialize())
		{
			m_num_of_visible_texture += ui->GetNumOfVisibleTexture();
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
