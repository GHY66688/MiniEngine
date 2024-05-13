#pragma once

#ifdef MG_PLATFORM_WINDOWS

extern MG::Application* MG::CreateApplication();


int main(int argc, char** argv)
{
	MG::Log::Init();
	
	MG_PROFILE_BEGIN_SESSION("Startup", "MG_Profile_Startup.json");
	auto app = MG::CreateApplication();
	MG_PROFILE_END_SESSION();

	MG_PROFILE_BEGIN_SESSION("Runtime", "MG_Profile_Runtime.json");
	app->Run();
	MG_PROFILE_END_SESSION();

	MG_PROFILE_BEGIN_SESSION("ShutDown", "MG_Profile_ShutDown.json");
	delete app;
	MG_PROFILE_END_SESSION();
}

#endif // MG_PLATFORM_WINDOWS
