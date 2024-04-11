#pragma once


namespace MG {
	//interface
	class GraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};

}	//end MG