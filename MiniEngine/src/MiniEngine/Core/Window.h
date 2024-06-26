#pragma once

#include"MGpch.h"

#include"MiniEngine/Core/Core.h"
#include"MiniEngine/Events/Event.h"

namespace MG {

	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "Mini Engine",
					unsigned int width = 1280,
					unsigned int height = 720)
			: Title(title), Width(width), Height(height)
		{
		}

	};
	

	//Interface
	class MG_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		//return GLFWwindow
		virtual void* GetNativeWindow() const = 0;

		//在不同平台上实现以下函数
		static Window* Create(const WindowProps& props = WindowProps());

	};


}	//end MG