#include"MGpch.h"

#include "Application.h"

#include"Input.h"

//temporate
#include <GLFW/glfw3.h>


namespace MG {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;


	

	Application::Application()
	{
		MG_CORE_ASSERT(!s_Instance, "Application already exists!");

		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_Window->SetVSync(false);

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		
	}
	Application::~Application()
	{
	}
	void Application::Run()
	{
		float time = (float)glfwGetTime();		//platform::GetTime()
		//TimeStep实际就一个float
		//当前帧的时间减去上一帧的时间 便是DeltaTime
		TimeStep timestep = time - m_LastFrameTime;
		m_LastFrameTime = time;

		//MG_CORE_WARN("Initialized Log!");
		//MG_CLIENT_TRACE("Hello");
		while (m_Runing)
		{

			

			
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(timestep);


			m_ImGuiLayer->Begin();
			//每一层进行更新
			for (Layer* layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}

			m_ImGuiLayer->End();


			m_Window->OnUpdate();
		}
	}


	//执行事件输出
	void Application::OnEvent(Event& e)
	{
		//如果事件类型为关闭窗口，则执行OnWindowClose
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		//MG_CORE_INFO("{0}", e);

		//反向遍历，当该事件已经处理完毕，则不再遍历
		//例如Layer1已经处理了该事件，并将该事件标记为Handled；Layer2就无需进行处理
		//由于每一种事件可能只对一层Layer有效；
		//此举是为了确保每一种事件都能够传给有效的最上层Layer进行处理，并且避免重复处理
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.m_Handled)
			{
				break;
			}
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	//关闭窗口
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Runing = false;
		return true;
	}
}	//end MG
