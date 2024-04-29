#include "MGpch.h"

#include "Renderer2D.h"

#include <memory>

#include <glm/gtc/matrix_transform.hpp>

#include "RenderCommand.h"

#include "Shader.h"
#include "VertexArray.h"




namespace MG {
	
	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();

		float vertices2[4 * 5] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		s_Data->QuadVertexArray = VertexArray::Create();
		Ref<VertexBuffer> squareVB;
		squareVB = VertexBuffer::Create(vertices2, sizeof(vertices2));
		squareVB->SetLayout({ { "a_Position", ShaderDataType::Float3 },
							  { "a_TexCoord", ShaderDataType::Float2 } });
		s_Data->QuadVertexArray->AddVertexBuffer(squareVB);

		uint32_t indices2[2 * 3] = {
			0, 1, 2,
			0, 2, 3
		};
		Ref<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(indices2, 6));

		s_Data->QuadVertexArray->SetIndexBuffer(squareIB);

		//创建白色texture
		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;	//for every channel
		s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_Texture", 0);	//指定默认的slot 0


	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}
	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());


	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x , position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		s_Data->TextureShader->SetFloat4("u_Color", color);
		//bind white texture
		s_Data->WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
		s_Data->TextureShader->SetMat4("u_Transform", transform);


		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		DrawQuad({ position.x , position.y, 0.0f }, size, texture);

	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		//set color to white
		s_Data->TextureShader->SetFloat4("u_Color", glm::vec4(1.0f));

		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
		s_Data->TextureShader->SetMat4("u_Transform", transform);



		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	//void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::mat4 rotation, const glm::vec4& color)
	//{
	//	s_Data->FlatColorShader->Bind();
	//	s_Data->FlatColorShader->SetFloat4("u_Color", color);

	//	glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
	//	s_Data->FlatColorShader->SetMat4("u_Transform", transform);


	//	s_Data->QuadVertexArray->Bind();
	//	RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	//}
}