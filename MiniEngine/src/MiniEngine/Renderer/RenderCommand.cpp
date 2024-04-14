#include "MGpch.h"

#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace MG {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();

}	//end MG