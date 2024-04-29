#pragma once


#include "MiniEngine/Core/Input.h"

namespace MG {

	class WindowsInput : public Input
	{
	protected:
		//virtual ��ʵ����ʡ��
		virtual bool IsKeyPressedImpl(int keycode) override;
		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};

}	//end MG