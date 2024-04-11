#include "MGpch.h"
#include "OpenGLContext.h"

#include<glad/glad.h>
#include<GLFW/glfw3.h>


namespace MG {

	OpenGLContext::OpenGLContext(GLFWwindow* windowhandle)
		: m_WindowHandle(windowhandle)
	{
		MG_CORE_ASSERT(windowhandle, "Window Handle is null");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		MG_CORE_ASSERT(status, "Failed to initialize Glad!");

		MG_CORE_INFO("OpenGL Render: {0}  {1}", (const char*)glGetString(GL_VENDOR), (const char*)glGetString(GL_RENDERER));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}	//end MG
