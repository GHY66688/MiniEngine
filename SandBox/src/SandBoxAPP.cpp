#include"MGpch.h"

#include"MiniEngine.h"
#include<iostream>

class SandBox : public MG::Application
{
public:
	SandBox() 
	{

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