#include "MGpch.h"

#include "VertexArray.h"
#include"Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace MG {



	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:
		{
			MG_CORE_ASSERT(false, "RendererAPI is currently not supported!");
			return nullptr;
		}
		case RendererAPI::OpenGL:
		{
			return new OpenGLVertexArray();
		}
		}

		MG_CORE_ASSERT(false, "unknown RendererAPI!");
		return nullptr;
	}

}	//end MG