#include"MGpch.h"

#include "Application.h"

#include"Input.h"

#include<glad/glad.h>

namespace MG {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;


	

	Application::Application()
	{
		MG_CORE_ASSERT(!s_Instance, "Application already exists!");

		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		m_VertexArray.reset(VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.2f, 0.4f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.2f, 0.4f, 1.0f,
			 0.0f,  0.5f, 0.0f, 1.0f, 0.2f, 0.4f, 1.0f
		};

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		vertexBuffer->SetLayout({
			{"a_Position", ShaderDataType::Float3},
			{"a_Color", ShaderDataType::Float4}
			});
		
		m_VertexArray->AddVertexBuffer(vertexBuffer);


		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		m_VertexArray->SetIndexBuffer(indexBuffer);


		float vertices2[4 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		m_SquareVA.reset(VertexArray::Create());
		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(vertices2, sizeof(vertices2)));
		squareVB->SetLayout({ { "a_Position", ShaderDataType::Float3 } });
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t indices2[2 * 3] = {
			0, 1, 2,
			0, 2, 3
		};
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(indices2, 6));

		m_SquareVA->SetIndexBuffer(squareIB);


		std::string vertexSrc = R"(
			#version 330 core
			layout(location = 0) in vec3 a_Position;
			layout(location = 0) in vec4 a_Color;
			out vec3 va_Position;
			out vec4 v_Color;
			void main()
			{
				v_Color = a_Color;
				va_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);
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


		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));

		////////////////////////////////////////////////////////
		//test squareVA
		std::string vertexSrc2 = R"(
			#version 330 core
			layout(location = 0) in vec3 a_Position;
			out vec3 va_Position;
			void main()
			{
				va_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);
			}
		)";


		std::string fragmentSrc2 = R"(
			#version 330 core
			layout(location = 0) out vec4 color;
			in vec3 va_Position;
			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";


		m_Shader2.reset(new Shader(vertexSrc2, fragmentSrc2));
	}
	Application::~Application()
	{
	}
	void Application::Run()
	{
		MG_CORE_WARN("Initialized Log!");
		MG_CLIENT_TRACE("Hello");
		while (m_Runing)
		{
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);


			m_Shader2->Bind();
			m_SquareVA->Bind();
			glDrawElements(GL_TRIANGLES, m_SquareVA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);


			m_Shader->Bind();
			m_VertexArray->Bind();
			glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();


			m_ImGuiLayer->Begin();
			//每一层进行更新
			for (Layer* layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}

			m_ImGuiLayer->End();


			m_Window->OnUpdate();
		}
	}


	//执行事件输出
	void Application::OnEvent(Event& e)
	{
		//如果事件类型为关闭窗口，则执行OnWindowClose
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		//MG_CORE_INFO("{0}", e);

		//反向遍历，当该事件已经处理完毕，则不再遍历
		//例如Layer1已经处理了该事件，并将该事件标记为Handled；Layer2就无需进行处理
		//由于每一种事件可能只对一层Layer有效；
		//此举是为了确保每一种事件都能够传给有效的最上层Layer进行处理，并且避免重复处理
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.m_Handled)
			{
				break;
			}
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	//关闭窗口
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Runing = false;
		return true;
	}
}	//end MG
