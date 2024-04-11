#pragma once

#include"MiniEngine/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace MG {

	class OpenGLContext : public MG::GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowhandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_WindowHandle;
	};

}	//end MG

