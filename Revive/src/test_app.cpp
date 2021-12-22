#include <client_fw.h>
#include <client/core/entry_point.h>

namespace revive
{
	class TestApp : public client_fw::Application
	{
	public:
		TestApp() : Application("Test App")
		{

		}

		bool Initialize() override
		{
			bool result = Application::Initialize();

			if (result)
			{
				LOG_INFO("Welcome to Test App");
			}

			return result;
		}

		virtual ~TestApp()
		{

		}
	};
}

client_fw::UPtr<client_fw::Application> client_fw::CreateApplication()
{
	return client_fw::CreateUPtr<revive::TestApp>();
}