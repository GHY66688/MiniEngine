#include "MGpch.h"

#include "VertexArray.h"
#include"Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace MG {



	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
		{
			MG_CORE_ASSERT(false, "RendererAPI is currently not supported!");
			return nullptr;
		}
		case RendererAPI::API::OpenGL:
		{
			return std::make_shared<OpenGLVertexArray>();
		}
		}

		MG_CORE_ASSERT(false, "unknown RendererAPI!");
		return nullptr;
	}

}	//end MG