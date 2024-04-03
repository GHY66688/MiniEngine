#pragma once

#include "Event.h"


namespace MG {

	class MG_API KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:
		KeyEvent(int KeyCode) : m_KeyCode(KeyCode) {}

		int m_KeyCode;
	};

	class MG_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int repeatcount) : KeyEvent(keycode), m_RepeatCount(repeatcount) {}

		inline int GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}
		//通过宏定义，传入KeyPressed这个参数，便可以直接自动生成对应的重写过的虚函数，使得KeyPressedEvent能够实例化
		//没有实现全部纯虚函数，则无法实例化
		//##KeyPressed 相当于 KeyPressed
		//#KeyPressed 相当于 "KeyPressed"
		EVENT_CLASS_TYPE(KeyPressed)


	private:
		int m_RepeatCount;
	};

	class MG_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)



	};


}	//end MG