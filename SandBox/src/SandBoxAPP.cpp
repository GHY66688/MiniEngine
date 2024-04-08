#include"MGpch.h"

#include"MiniEngine.h"
#include<iostream>

class ExampleLayer : public MG::Layer
{
public:
	ExampleLayer() : Layer("Example") {}

	void OnUpdate() override
	{
		MG_CLIENT_INFO("ExampleLayer::Update");
	}

	void OnEvent(MG::Event& event) override
	{
		MG_CLIENT_TRACE("{0}", event);
	}
};


class SandBox : public MG::Application
{
public:
	SandBox() 
	{
		//PushLayer(new ExampleLayer());
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