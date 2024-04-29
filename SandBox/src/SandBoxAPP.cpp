#include "MGpch.h"

#include <MiniEngine.h>
#include <MiniEngine/Core/EntryPoint.h>



#include"imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

#include "SandBox2D.h"





class ExampleLayer : public MG::Layer
{
public:
	ExampleLayer() : Layer("Example"), m_CameraController(1280.0f / 720.0f)
	{
		m_VertexArray = MG::VertexArray::Create();

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.2f, 0.4f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.2f, 0.4f, 1.0f,
			 0.0f,  0.5f, 0.0f, 1.0f, 0.2f, 0.4f, 1.0f
		};

		MG::Ref<MG::VertexBuffer> vertexBuffer;
		vertexBuffer = MG::VertexBuffer::Create(vertices, sizeof(vertices));

		vertexBuffer->SetLayout({
			{"a_Position", MG::ShaderDataType::Float3},
			{"a_Color", MG::ShaderDataType::Float4}
			});

		m_VertexArray->AddVertexBuffer(vertexBuffer);


		uint32_t indices[3] = { 0, 1, 2 };
		MG::Ref<MG::IndexBuffer> indexBuffer;
		indexBuffer.reset(MG::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		m_VertexArray->SetIndexBuffer(indexBuffer);


		float vertices2[4 * 5] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		m_SquareVA = MG::VertexArray::Create();
		MG::Ref<MG::VertexBuffer> squareVB;
		squareVB = MG::VertexBuffer::Create(vertices2, sizeof(vertices2));
		squareVB->SetLayout({ { "a_Position", MG::ShaderDataType::Float3 },
							  { "a_TexCoord", MG::ShaderDataType::Float2 } });
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t indices2[2 * 3] = {
			0, 1, 2,
			0, 2, 3
		};
		MG::Ref<MG::IndexBuffer> squareIB;
		squareIB.reset(MG::IndexBuffer::Create(indices2, 6));

		m_SquareVA->SetIndexBuffer(squareIB);


		std::string vertexSrc = R"(
			#version 330 core
			layout(location = 0) in vec3 a_Position;
			layout(location = 0) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 va_Position;
			out vec4 v_Color;
			void main()
			{
				v_Color = a_Color;
				va_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";


		std::string fragmentSrc = R"(
			#version 330 core
			layout(location = 0) out vec4 color;
			in vec3 va_Position;
			in vec4 v_Color;
			void main()
			{
				color = vec4(va_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";


		m_Shader = MG::Shader::Create("TriangleVertexcolor", vertexSrc, fragmentSrc);

		////////////////////////////////////////////////////////
		//test squareVA
		std::string vertexSrc2 = R"(
			#version 330 core
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 va_Position;
			void main()
			{
				va_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";


		std::string fragmentSrc2 = R"(
			#version 330 core
			layout(location = 0) out vec4 color;
			in vec3 va_Position;

			uniform vec3 u_Color;			

			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

		m_Shader2 = MG::Shader::Create("SquareUniformColor", vertexSrc2, fragmentSrc2);

		auto TextureShader = m_ShaderLibrary.Load("Texture", "assets/shaders/Texture.glsl");

		//m_TextureShader = MG::Shader::Create("assets/shaders/Texture.glsl");

		m_Texture = MG::Texture2D::Create("assets/textures/PNG_test.png");
		m_TextureAlpha = MG::Texture2D::Create("assets/textures/PNG_test1.png");


		std::dynamic_pointer_cast<MG::OpenGLShader>(TextureShader)->Bind();
		std::dynamic_pointer_cast<MG::OpenGLShader>(TextureShader)->UploadUniformInt("u_Texture", 0);	//0代表0号插槽
	}

	//轮询，每帧都会输出
	void OnUpdate(MG::TimeStep ts) override
	{
		//Update
		m_CameraController.OnUpdate(ts);


		//Render
		MG::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		MG::RenderCommand::Clear();


		MG::Renderer::BeginScene(m_CameraController.GetCamera());

		//设置大小为原来的0.1倍
		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));


		//std::dynamic_pointer_cast<MG::OpenGLShader>(m_Shader2)->Bind();
		//std::dynamic_pointer_cast<MG::OpenGLShader>(m_Shader2)->UploadUniformFloat3("u_Color", m_SquareColor);

		//std::dynamic_pointer_cast<MG::OpenGLShader>(m_TextureShader)->Bind();
		//std::dynamic_pointer_cast<MG::OpenGLShader>(m_TextureShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		//for (int i = 0; i < 5; ++i)
		//{
		//	
		//	//设置出生位置， 可以在两个正方形中间留下0.01f的间隔
		//	glm::vec3 pos(i * 0.11f, 0.0f, 0.0f);
		//	glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
		//	MG::Renderer::Submit(m_Shader2, m_SquareVA, transform);
		//}

		//三角形
		//MG::Renderer::Submit(m_Shader, m_VertexArray);

		//正方形
		auto TextureShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();
		MG::Renderer::Submit(TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		m_TextureAlpha->Bind();
		MG::Renderer::Submit(TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));



		MG::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));

		ImGui::End();
	}

	//触发事件时输出
	void OnEvent(MG::Event& event) override
	{
		m_CameraController.OnEvent(event);

	}


private:
	//TODO:将ShaderLibrary提取到Renderer中，在初始化Renderer时便直接加载Shader
	MG::ShaderLibrary m_ShaderLibrary;

	MG::Ref<MG::VertexArray> m_VertexArray;
	MG::Ref<MG::Shader> m_Shader;

	MG::Ref<MG::VertexArray> m_SquareVA;
	MG::Ref<MG::Shader> m_Shader2;

	MG::Ref<MG::Texture2D> m_Texture, m_TextureAlpha;

	MG::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };

	//glm::vec3 m_ModelPosition;
	//float m_ModelMoveSpeed = 0.01f;		//model不用取反

};


class SandBox : public MG::Application
{
public:
	//先构建Application，创建上下文，然后创建ExampleLayer
	SandBox() 
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new SandBox2D());
	}
	~SandBox() 
	{

	}
};

MG::Application* MG::CreateApplication()
{
	//std::cout << "Welcome to Mini Engine\n";
	return new SandBox();
}