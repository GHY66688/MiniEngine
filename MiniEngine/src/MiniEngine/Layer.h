#pragma once

#include"MiniEngine/Core.h"
#include"MiniEngine/Events/Event.h"

#include "Core/TimeStep.h"

namespace MG {
	class MG_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(TimeStep ts) {}
		virtual void OnEvent(Event& e) {}
		virtual void OnImGuiRender() {}

		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;

		//TODO 加入启用和禁用layer
	};

}	//end MG

