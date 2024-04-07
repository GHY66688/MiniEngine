#pragma once

#include "MiniEngine/Window.h"

#include<GLFW/glfw3.h>

namespace MG {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		//将OnEvent函数进行SetEventCallback,会自动调用OnEvent
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }


		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			//EventCallbackFn = std::function<void(Event&)> 会调用参数为Event&,返回值为void的函数
			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

}

