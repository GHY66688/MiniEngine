#pragma once

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "MiniEngine/Events/Event.h"
#include "MiniEngine/Events/ApplicationEvent.h"

#include "MiniEngine/Core/TimeStep.h"

#include "MiniEngine/ImGui/ImGuiLayer.h"




namespace MG {

	//����ģʽ
	class MG_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);


		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);


	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Runing = true;
		bool m_Minimized = false;	//��С���󣬲���Ⱦ��ImGui�����layer����ԼGPU����
		LayerStack m_LayerStack;
		//���һ֡����ʱ��
		float m_LastFrameTime = 0.0f;



	private:
		static Application* s_Instance;
	};

	//To be defined in CLIENT
	Application* CreateApplication();

}	//end MG

