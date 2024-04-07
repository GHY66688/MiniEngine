#pragma once

#ifdef MG_PLATFORM_WINDOWS

extern MG::Application* MG::CreateApplication();


int main(int argc, char** argv)
{
	MG::Log::Init();
	

	auto app = MG::CreateApplication();
	app->Run();
	delete app;
}

#endif // MG_PLATFORM_WINDOWS
