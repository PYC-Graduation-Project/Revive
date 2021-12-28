#include <client_fw.h>
#include <client/core/entry_point.h>

namespace revive
{
	class ClientTestApp : public client_fw::Application
	{
	public:
		ClientTestApp() : Application(L"Client Test App")
		{

		}

		bool Initialize() override
		{
			bool result = Application::Initialize();

			if (result)
			{
				LOG_INFO("Welcome to Client Test App");
			}

			return result;
		}

		void Shutdown() override
		{
			Application::Shutdown();

			LOG_INFO("Good Bye");
		}

		virtual ~ClientTestApp()
		{

		}
	};
}

client_fw::UPtr<client_fw::Application> client_fw::CreateApplication()
{
	return client_fw::CreateUPtr<revive::ClientTestApp>();
}