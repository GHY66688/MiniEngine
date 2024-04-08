#pragma once

#include"MiniEngine/Core.h"
#include"MiniEngine/Events/Event.h"

namespace MG {
	class MG_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& e) {}

		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;

		//TODO 加入启用和禁用layer
	};

}	//end MG

