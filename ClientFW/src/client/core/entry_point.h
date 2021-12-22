#pragma once

extern client_fw::UPtr<client_fw::Application> client_fw::CreateApplication();

int main(int argc, char** argv)
{
	client_fw::Log::Initialize();

	const auto& app = client_fw::CreateApplication();
	if (app->Initialize())
	{
		app->Run();
		app->Shutdown();
	}
	else 
	{
		LOG_ERROR("Could not initialize app");
	}
}