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

int main()
{
	SandBox* sanbox = new SandBox();
	sanbox->Run();
	delete sanbox;
}