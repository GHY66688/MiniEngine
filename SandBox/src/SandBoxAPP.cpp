#include"MGpch.h"

#include"MiniEngine.h"
#include<iostream>

#include"imgui/imgui.h"




class ExampleLayer : public MG::Layer
{
public:
	ExampleLayer() : Layer("Example") 
	{
	}

	//轮询，每帧都会输出
	void OnUpdate() override
	{
		if (MG::Input::IsKeyPressed(MG_KEY_TAB))
		{
			MG_CLIENT_TRACE("Tab is pressed！");
		}
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello world");
		ImGui::End();
	}

	//触发事件时输出
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