#include"MGpch.h"

#include "Application.h"

#include"Input.h"

#include<glad/glad.h>

namespace MG {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		MG_CORE_ASSERT(!s_Instance, "Application already exists!");

		s_Instance = this;

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

			//ÿһ����и���
			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate();
			}


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
