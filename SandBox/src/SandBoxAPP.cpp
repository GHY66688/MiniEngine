#include"MiniEngine.h"


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
	return new SandBox();
}