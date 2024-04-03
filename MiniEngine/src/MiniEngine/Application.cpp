#include"MGpch.h"

#include "Application.h"
#include"Events/ApplicationEvent.h"
#include"Log.h"

namespace MG {
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}
	Application::~Application()
	{
	}
	void Application::Run()
	{
		while (m_Runing)
		{
			m_Window->OnUpdate();
		}
	}
}	//end MG
