#pragma once
#include "client/object/core/base_object_manager.h"

namespace client_fw
{
	class UserInterface;

	class UserInterfaceManager : public IBaseObjectManager
	{
	public:
		UserInterfaceManager();
		virtual ~UserInterfaceManager() = default;

		UserInterfaceManager(const UserInterfaceManager&) = delete;
		UserInterfaceManager& operator=(const UserInterfaceManager&) = delete;

		virtual void Shutdown() override;
		virtual void Update(float delta_time) override;

		bool RegisterUserInterface(const SPtr<UserInterface>& ui);

	private:
		static UserInterfaceManager* s_ui_manager;
		std::vector<SPtr<UserInterface>> m_user_interfaces;

	public:
		static UserInterfaceManager* GetUIManager() { return s_ui_manager; }
		const std::vector<SPtr<UserInterface>>& GetUserInterfaces() const { return m_user_interfaces; }
	};
}


