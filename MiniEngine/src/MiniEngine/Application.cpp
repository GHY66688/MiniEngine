#include"MGpch.h"

#include "Application.h"

#include"Input.h"

#include<glad/glad.h>

namespace MG {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;


	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case MG::ShaderDataType::Float:		return GL_FLOAT;
		case MG::ShaderDataType::Float2:	return GL_FLOAT;
		case MG::ShaderDataType::Float3:	return GL_FLOAT;
		case MG::ShaderDataType::Float4:	return GL_FLOAT;
		case MG::ShaderDataType::Mat3:		return GL_FLOAT;
		case MG::ShaderDataType::Mat4:		return GL_FLOAT;
		case MG::ShaderDataType::Int:		return GL_INT;
		case MG::ShaderDataType::Int2:		return GL_INT;
		case MG::ShaderDataType::Int3:		return GL_INT;
		case MG::ShaderDataType::Int4:		return GL_INT;
		case MG::ShaderDataType::Bool:		return GL_BOOL;
		}
		MG_CORE_ASSERT(false, "Unknown Shader Data Type! [ShaderDataTypeToOpenGLBaseType]");
		return 0;
	}

	Application::Application()
	{
		MG_CORE_ASSERT(!s_Instance, "Application already exists!");

		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.2f, 0.4f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.2f, 0.4f, 1.0f,
			 0.0f,  0.5f, 0.0f, 1.0f, 0.2f, 0.4f, 1.0f
		};

		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		//设置layout，并且快速释放，提高可读性
		{
			BufferLayout layout = {
				{"a_Position", ShaderDataType::Float3},
				{"a_Color", ShaderDataType::Float4}
			};

			m_VertexBuffer->SetLayout(layout);
		}
		uint32_t index = 0;
		const auto& layout = m_VertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);

			glVertexAttribPointer(index, 
				element.GetComponentCount(), 
				ShaderDataTypeToOpenGLBaseType(element.Type), 
				element.Normalized ? GL_TRUE : GL_FALSE, 
				layout.GetStride(),
				(const void*)element.Offset);
			index++;

		}



		uint32_t indices[3] = { 0, 1, 2 };
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

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


			m_Shader->Bind();
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

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
