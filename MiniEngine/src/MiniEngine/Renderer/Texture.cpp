#include "MGpch.h"

#include <memory>

#include "Texture.h"
#include "VertexArray.h"
#include"Renderer.h"

#include "Platform/OpenGL/OpenGLTexture.h"

namespace MG {
	Ref<Texture2D> Texture2D::Create(const std::string& path)
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
				return std::make_shared<OpenGLTexture2D>(path);
			}
		}

		MG_CORE_ASSERT(false, "unknown RendererAPI!");
		return nullptr;
	}


}	//end MG