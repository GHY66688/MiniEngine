#include "MGpch.h"

#include "Shader.h"
#include"Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace MG {



	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
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
			return new OpenGLShader(vertexSrc, fragmentSrc);
		}
		}

		MG_CORE_ASSERT(false, "unknown RendererAPI!");
		return nullptr;
	}

}	//end MG