#include"MGpch.h"

#include"MiniEngine.h"
#include<iostream>

#include"imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"





class ExampleLayer : public MG::Layer
{
public:
	ExampleLayer() : Layer("Example"), m_Camera({-1.6f, 1.6f, -0.9f, 0.9f}), m_CameraPosition(0.0f)
	{
		m_VertexArray.reset(MG::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.2f, 0.4f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.2f, 0.4f, 1.0f,
			 0.0f,  0.5f, 0.0f, 1.0f, 0.2f, 0.4f, 1.0f
		};

		std::shared_ptr<MG::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(MG::VertexBuffer::Create(vertices, sizeof(vertices)));

		vertexBuffer->SetLayout({
			{"a_Position", MG::ShaderDataType::Float3},
			{"a_Color", MG::ShaderDataType::Float4}
			});

		m_VertexArray->AddVertexBuffer(vertexBuffer);


		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<MG::IndexBuffer> indexBuffer;
		indexBuffer.reset(MG::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		m_VertexArray->SetIndexBuffer(indexBuffer);


		float vertices2[4 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		m_SquareVA.reset(MG::VertexArray::Create());
		std::shared_ptr<MG::VertexBuffer> squareVB;
		squareVB.reset(MG::VertexBuffer::Create(vertices2, sizeof(vertices2)));
		squareVB->SetLayout({ { "a_Position", MG::ShaderDataType::Float3 } });
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t indices2[2 * 3] = {
			0, 1, 2,
			0, 2, 3
		};
		std::shared_ptr<MG::IndexBuffer> squareIB;
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


		m_Shader.reset(MG::Shader::Create(vertexSrc, fragmentSrc));

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

			uniform vec4 u_Color;			

			void main()
			{
				color = u_Color;
			}
		)";


		m_Shader2.reset(MG::Shader::Create(vertexSrc2, fragmentSrc2));
	}

	//轮询，每帧都会输出
	void OnUpdate(MG::TimeStep ts) override
	{
		MG_CLIENT_TRACE("delta time:  {0}", ts);

		//////////////////////////////////////////////////////////////
		//camera move and rotate///////////////////////////////
		// //////////////////////////////////////////////////////////////
		//在按刷新率接收事件时，可以使得相机移动更加smooth
		//但是刷新率越高的显示器，相机移动速度越快，需要设置deltaTimeUpdate

		//加入else后，上面的判定会优先于else的判定
		if (MG::Input::IsKeyPressed(MG_KEY_LEFT) || MG::Input::IsKeyPressed(MG_KEY_A))
		{
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		}
		else if (MG::Input::IsKeyPressed(MG_KEY_RIGHT) || MG::Input::IsKeyPressed(MG_KEY_D))
		{
			m_CameraPosition.x += m_CameraMoveSpeed * ts;
		}

		if (MG::Input::IsKeyPressed(MG_KEY_UP) || MG::Input::IsKeyPressed(MG_KEY_W))
		{
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		}
		else if (MG::Input::IsKeyPressed(MG_KEY_DOWN) || MG::Input::IsKeyPressed(MG_KEY_S))
		{
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;
		}

		//rotation
		if (MG::Input::IsKeyPressed(MG_KEY_Q))
		{
			m_CameraRotation += m_CameraRotateSpeed * ts;
		}
		else if (MG::Input::IsKeyPressed(MG_KEY_E))
		{
			m_CameraRotation -= m_CameraRotateSpeed * ts;
		}

		////object position
		//if (MG::Input::IsKeyPressed(MG_KEY_J))
		//{
		//	m_ModelPosition.x -= m_ModelMoveSpeed * ts;
		//}
		//else if (MG::Input::IsKeyPressed(MG_KEY_L))
		//{
		//	m_ModelPosition.x += m_ModelMoveSpeed * ts;
		//}

		//if (MG::Input::IsKeyPressed(MG_KEY_I))
		//{
		//	m_ModelPosition.y += m_ModelMoveSpeed * ts;
		//}
		//else if (MG::Input::IsKeyPressed(MG_KEY_K))
		//{
		//	m_ModelPosition.y -= m_ModelMoveSpeed * ts;
		//}

		///////////////////////////////////////////////////////////
		//renderer/////////////////////////////////////////////////
		///////////////////////////////////////////////////////////
		MG::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		MG::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		MG::Renderer::BeginScene(m_Camera);

		//设置大小为原来的0.1倍
		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));


		std::dynamic_pointer_cast<MG::OpenGLShader>(m_Shader2)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int i = 0; i < 5; ++i)
		{
			
			//设置出生位置， 可以在两个正方形中间留下0.01f的间隔
			glm::vec3 pos(i * 0.11f, 0.0f, 0.0f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
			MG::Renderer::Submit(m_Shader2, m_SquareVA, transform);
		}


		//MG::Renderer::Submit(m_Shader, m_VertexArray);


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
		//调用事件回调函数，更新相机Position，做到移动相机
		/*MG::EventDispatcher dispatch(event);
		dispatch.Dispatch<MG::KeyPressedEvent>(MG_BIND_EVENT_FN(ExampleLayer::OnKeyPressedEvent));*/
	}

	////由于是事件回调，所以在移动时会按照按键的触发间隔进行，而不是以实际设定的刷新率更新，导致相机移动卡顿
	//bool OnKeyPressedEvent(MG::KeyPressedEvent& event)
	//{
	//	if (event.GetKeyCode() == MG_KEY_LEFT || event.GetKeyCode() == MG_KEY_A)
	//	{
	//		m_CameraPosition.x -= m_CameraSpeed;
	//	}
	//	if (event.GetKeyCode() == MG_KEY_RIGHT || event.GetKeyCode() == MG_KEY_D)
	//	{
	//		m_CameraPosition.x += m_CameraSpeed;
	//	}
	//	if (event.GetKeyCode() == MG_KEY_UP || event.GetKeyCode() == MG_KEY_W)
	//	{
	//		m_CameraPosition.y += m_CameraSpeed;
	//	}
	//	if (event.GetKeyCode() == MG_KEY_DOWN || event.GetKeyCode() == MG_KEY_S)
	//	{
	//		m_CameraPosition.y -= m_CameraSpeed;
	//	}
	//	//z轴前后移动， 但因为是正交相机，所以大小不会发生变化，超过一定距离会直接消失
	//	//if (event.GetKeyCode() == MG_KEY_LEFT_SHIFT)
	//	//{
	//	//	m_CameraPosition.z -= m_CameraSpeed;
	//	//}
	//	//if (event.GetKeyCode() == MG_KEY_LEFT_CONTROL)
	//	//{
	//	//	m_CameraPosition.z += m_CameraSpeed;
	//	//}
	//	return false;
	//}

private:
	std::shared_ptr<MG::VertexArray> m_VertexArray;
	std::shared_ptr<MG::Shader> m_Shader;

	std::shared_ptr<MG::VertexArray> m_SquareVA;
	std::shared_ptr<MG::Shader> m_Shader2;

	MG::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = -0.01f;	//1s移动的幅度
	float m_CameraRotation = 0.0f;
	float m_CameraRotateSpeed = -0.05f;	//一s旋转的幅度

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
		PushLayer(new ExampleLayer());
		//将ImGuiLayer设为Application的private变量，自动添加
		//PushOverlay(new MG::ImGuiLayer());
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