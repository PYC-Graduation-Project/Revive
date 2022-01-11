#include <include/client_fw.h>
#include <client/core/entry_point.h>

namespace revive
{
	class Revive : public client_fw::Application
	{
	public:
		Revive() : Application(L"Revive")
		{

		}

		bool Initialize() override
		{
			bool result = Application::Initialize();

			if (result)
			{
				LOG_INFO("Welcome to Revive Application");
			}

			return result;
		}

		void Shutdown() override
		{
			Application::Shutdown();

			LOG_INFO("Good Bye");
		}

		virtual ~Revive()
		{

		}
	};
}

client_fw::UPtr<client_fw::Application> client_fw::CreateApplication()
{
	return client_fw::CreateUPtr<revive::Revive>();
}