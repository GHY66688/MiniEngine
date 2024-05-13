#pragma once

#include "MiniEngine/Core/Layer.h"

#include "MiniEngine/Events/ApplicationEvent.h"
#include "MiniEngine/Events/KeyEvent.h"
#include "MiniEngine/Events/MouseEvent.h"

namespace MG {

	class MG_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		//virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};

}