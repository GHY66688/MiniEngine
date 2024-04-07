#pragma once


#include"Core.h"
#include"spdlog/spdlog.h"
#include"spdlog/fmt/ostr.h"


namespace MG {
	class MG_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }

		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
	};

}	//end MG


//Log��ɫ��warn��info�ȿ���
//__VA_ARGS__��ʾ�����ɱ䣬������Ĳ���ֵ���Խ��иı�(�����ǲ��������ɱ�)
//����core��
#define MG_CORE_TRACE(...)	::MG::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define MG_CORE_INFO(...)	::MG::Log::GetCoreLogger()->info(__VA_ARGS__)
#define MG_CORE_WARN(...)	::MG::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define MG_CORE_ERROR(...)	::MG::Log::GetCoreLogger()->error(__VA_ARGS__)
//#define MG_CORE_FATAL(...)	::MG::Log::GetCoreLogger()->fatal(__VA_ARGS__)

//����client��
#define MG_CLIENT_TRACE(...)	::MG::Log::GetClientLogger()->trace(__VA_ARGS__)
#define MG_CLIENT_INFO(...)		::MG::Log::GetClientLogger()->info(__VA_ARGS__)
#define MG_CLIENT_WARN(...)		::MG::Log::GetClientLogger()->warn(__VA_ARGS__)
#define MG_CLIENT_ERROR(...)	::MG::Log::GetClientLogger()->error(__VA_ARGS__)
//#define MG_CLIENT_FATAL(...)	::MG::Log::GetClientLogger()->fatal(__VA_ARGS__)


