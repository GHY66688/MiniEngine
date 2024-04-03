#include"MGpch.h"

#include "Log.h"
#include"spdlog/sinks/stdout_color_sinks.h"


namespace MG {


	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		//设定Log样式，指定颜色，时间戳，core/client，内容
		spdlog::set_pattern("%^[%T] %n: %v%$");

		//设置设备前缀
		s_CoreLogger = spdlog::stdout_color_mt("MiniEngine");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
	}

}	//end MG
