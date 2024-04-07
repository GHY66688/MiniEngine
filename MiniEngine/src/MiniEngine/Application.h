#pragma once

#include"Core.h"
#include"Events/Event.h"
#include"Window.h"
#include"Events/ApplicationEvent.h"


namespace MG {
	class MG_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

	private:
		bool OnWindowClose(WindowCloseEvent& e);


		std::unique_ptr<Window> m_Window;
		bool m_Runing = true;
	};

	//To be defined in CLIENT
	Application* CreateApplication();

}	//end MG


