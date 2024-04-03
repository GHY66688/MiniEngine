#include"MGpch.h"

#include "Application.h"
#include"Events/ApplicationEvent.h"
#include"Log.h"

namespace MG {
	Application::Application()
	{
	}
	Application::~Application()
	{
	}
	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		MG_CLIENT_TRACE(e);

		while (true);
	}
}	//end MG
