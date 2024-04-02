#pragma once

#include"Core.h"

namespace MG {
	class MG_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	//To be defined in CLIENT
	Application* CreateApplication();

}	//end MG


