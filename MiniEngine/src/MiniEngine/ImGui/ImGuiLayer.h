#pragma once

#include"MiniEngine/Layer.h"

namespace MG {


	class MG_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& e);
		
	private:
		float m_Time = 0.0f;
	};


}	//end MG

