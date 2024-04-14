#pragma once

#include"MiniEngine/Layer.h"
#include"MiniEngine/Events/MouseEvent.h"
#include"MiniEngine/Events/KeyEvent.h"
#include"MiniEngine/Events/ApplicationEvent.h"

namespace MG {


	class MG_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(TimeStep ts) override;
		virtual void OnImGuiRender() override;

		//begin ImGui
		void Begin();
		//end ImGui
		void End();


	private:
		float m_Time = 0.0f;
	};


}	//end MG

