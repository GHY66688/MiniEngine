#include "MGpch.h"

#include "Buffer.h" 
		 
#include "MiniEngine/Core/Core.h"
		 
#include "Renderer.h"
		 
#include "Platform/OpenGL/OpenGLBuffer.h"

//TODO ͨ��ʹ��#ifdef �ں��ʵ�ƽ̨ѡ����ʵ�ͼ��API�����Լ��ٱ���ʱ��

namespace MG {
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: MG_CORE_ASSERT(false, "RendererAPI is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLVertexBuffer>(size);
		}

		MG_CORE_ASSERT(false, "unknown RendererAPI!");
		return nullptr;
	}
	//����ʹ������ͼ��API
	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: MG_CORE_ASSERT(false, "RendererAPI is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLVertexBuffer>(vertices, size);
		}

		MG_CORE_ASSERT(false, "unknown RendererAPI!");
		return nullptr;
	}


	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
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
			return std::make_shared<OpenGLIndexBuffer>(indices, count);
		}
		}

		MG_CORE_ASSERT(false, "unknown RendererAPI!");
		return nullptr;
	}
}	//end MG