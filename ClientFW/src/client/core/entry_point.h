#pragma once

extern client_fw::UPtr<client_fw::Application> client_fw::CreateApplication();

int main(int argc, char** argv)
{
	const auto& app = client_fw::CreateApplication();
	if (app->Initialize())
	{
		app->Run();
		app->Shutdown();
	}
}