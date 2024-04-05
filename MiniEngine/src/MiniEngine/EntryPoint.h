#pragma once

#ifdef MG_PLATFORM_WINDOWS

extern MG::Application* MG::CreateApplication();


int main(int argc, char** argv)
{
	MG::Log::Init();
	
	std::cout << "test1\n";

	//MG_CLIENT_TRACE("Initialized Log!");

	std::cout << "finish\n";

	auto app = MG::CreateApplication();
	app->Run();
	delete app;
}

#endif // MG_PLATFORM_WINDOWS
