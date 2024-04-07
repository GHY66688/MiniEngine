#include"MGpch.h"

#include "Application.h"

#include<GLFW/glfw3.h>

namespace MG {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	}
	Application::~Application()
	{
	}
	void Application::Run()
	{
		MG_CORE_WARN("Initialized Log!");
		MG_CLIENT_TRACE("Hello");
		while (m_Runing)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}


	//执行事件输出
	void Application::OnEvent(Event& e)
	{
		//如果事件类型为关闭窗口，则执行OnWindowClose
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		MG_CORE_INFO("{0}", e);
	}

	//关闭窗口
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Runing = false;
		return true;
	}
}	//end MG
