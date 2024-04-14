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
		//TimeStepʵ�ʾ�һ��float
		//��ǰ֡��ʱ���ȥ��һ֡��ʱ�� ����DeltaTime
		TimeStep timestep = time - m_LastFrameTime;
		m_LastFrameTime = time;

		//MG_CORE_WARN("Initialized Log!");
		//MG_CLIENT_TRACE("Hello");
		while (m_Runing)
		{

			

			
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(timestep);


			m_ImGuiLayer->Begin();
			//ÿһ����и���
			for (Layer* layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}

			m_ImGuiLayer->End();


			m_Window->OnUpdate();
		}
	}


	//ִ���¼����
	void Application::OnEvent(Event& e)
	{
		//����¼�����Ϊ�رմ��ڣ���ִ��OnWindowClose
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		//MG_CORE_INFO("{0}", e);

		//��������������¼��Ѿ�������ϣ����ٱ���
		//����Layer1�Ѿ������˸��¼����������¼����ΪHandled��Layer2��������д���
		//����ÿһ���¼�����ֻ��һ��Layer��Ч��
		//�˾���Ϊ��ȷ��ÿһ���¼����ܹ�������Ч�����ϲ�Layer���д������ұ����ظ�����
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

	//�رմ���
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Runing = false;
		return true;
	}
}	//end MG
