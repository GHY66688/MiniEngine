#pragma once

#include "MiniEngine/Renderer/VertexArray.h"

namespace MG {

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();

		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		//use after VertexBuffer has create and SetLayout
		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;

		//可能不会有多个IB
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;


		inline virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffer() const override { return m_VertexBuffers; }
		inline virtual const Ref<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }

	private:
		uint32_t m_RendererID;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
	};

}	//end MG