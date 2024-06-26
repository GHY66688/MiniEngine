#pragma once

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "MiniEngine/Events/Event.h"
#include "MiniEngine/Events/ApplicationEvent.h"

#include "MiniEngine/Core/TimeStep.h"

#include "MiniEngine/ImGui/ImGuiLayer.h"




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
		bool OnWindowResize(WindowResizeEvent& e);


	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Runing = true;
		bool m_Minimized = false;	//最小化后，不渲染除ImGui以外的layer，节约GPU开销
		LayerStack m_LayerStack;
		//最后一帧所需时间
		float m_LastFrameTime = 0.0f;



	private:
		static Application* s_Instance;
	};

	//To be defined in CLIENT
	Application* CreateApplication();

}	//end MG


