#include"MGpch.h"

#include"MiniEngine.h"
#include<iostream>

class ExampleLayer : public MG::Layer
{
public:
	ExampleLayer() : Layer("Example") {}

	//��ѯ��ÿ֡�������
	void OnUpdate() override
	{
		if (MG::Input::IsKeyPressed(MG_KEY_TAB))
		{
			MG_CLIENT_TRACE("Tab is pressed��");
		}
	}

	//�����¼�ʱ���
	void OnEvent(MG::Event& event) override
	{
		//MG_CLIENT_TRACE("{0}", event);
	}
};


class SandBox : public MG::Application
{
public:
	SandBox() 
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new MG::ImGuiLayer());
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