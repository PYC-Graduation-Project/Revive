#include "stdafx.h"
#include "client/object/ui/core/user_interface_layer.h"
#include "client/object/ui/core/user_interface.h"

namespace client_fw
{
	UserInterfaceLayer::UserInterfaceLayer(const std::string& name)
		: m_name(name), m_layer_state(eUILayerState::kActive)
	{
	}

	bool UserInterfaceLayer::InitializeUILayer()
	{
		bool ret = false;
		ret = Initialize();
		m_is_initialized = true;
		return  ret;
	}

	void UserInterfaceLayer::ShutdownUILayer()
	{
		m_is_initialized = false;
		m_num_of_visible_texture = 0;
		Shutdown();

		for (const auto& ui : m_user_interfaces)
			ui->Shutdown();
		m_user_interfaces.clear();
	}

	void UserInterfaceLayer::UpdateUILayer(float delta_time)
	{
		for (const auto& ui : m_user_interfaces)
			ui->Update(delta_time);
		Update(delta_time);
	}

	bool UserInterfaceLayer::RegisterUserInterface(const SPtr<UserInterface>& ui)
	{
		if (m_is_initialized == false)
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
		else
		{
			LOG_WARN("Could not register user interface after initialization");
			return false;
		}
	}
}
