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

		//����layout�����ҿ����ͷţ���߿ɶ���
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
			//ÿһ����и���
			for (Layer* layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}

			m_ImGuiLayer->End();


			m_Window->OnUpdate();
		}
	}


	//ִ���¼����
	void Application::OnEvent(Event& e)
	{
		//����¼�����Ϊ�رմ��ڣ���ִ��OnWindowClose
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		//MG_CORE_INFO("{0}", e);

		//��������������¼��Ѿ�������ϣ����ٱ���
		//����Layer1�Ѿ������˸��¼����������¼����ΪHandled��Layer2��������д���
		//����ÿһ���¼�����ֻ��һ��Layer��Ч��
		//�˾���Ϊ��ȷ��ÿһ���¼����ܹ�������Ч�����ϲ�Layer���д������ұ����ظ�����
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

	//�رմ���
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Runing = false;
		return true;
	}
}	//end MG
