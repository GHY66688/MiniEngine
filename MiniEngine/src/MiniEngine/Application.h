#pragma once

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

#include "Core/TimeStep.h"

#include "ImGui/ImGuiLayer.h"




namespace MG {

	//单例模式
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

	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Runing = true;
		LayerStack m_LayerStack;
		//最后一帧所需时间
		float m_LastFrameTime = 0.0f;



	private:
		static Application* s_Instance;
	};

	//To be defined in CLIENT
	Application* CreateApplication();

}	//end MG


