#include "MGpch.h"

#include "Buffer.h" 
		 
#include "MiniEngine/Core/Core.h"
		 
#include "Renderer.h"
		 
#include "Platform/OpenGL/OpenGLBuffer.h"

//TODO ͨ��ʹ��#ifdef �ں��ʵ�ƽ̨ѡ����ʵ�ͼ��API�����Լ��ٱ���ʱ��

namespace MG {
	//����ʹ������ͼ��API
	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
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
				return std::make_shared<OpenGLVertexBuffer>(vertices, size);
			}
		}

		MG_CORE_ASSERT(false, "unknown RendererAPI!");
		return nullptr;
	}


	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
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
			return new OpenGLIndexBuffer(indices, count);
		}
		}

		MG_CORE_ASSERT(false, "unknown RendererAPI!");
		return nullptr;
	}
}	//end MG