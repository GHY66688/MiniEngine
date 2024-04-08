#pragma once

//设置平台
#ifdef MG_PLATFORM_WINDOWS
	#ifdef MG_BUILD_DLL
		#define MG_API __declspec(dllexport)
	#else 
		#define MG_API __declspec(dllimport)
	#endif // MG_BUILD_DLL
#else
	#error MiniEngine only support Windows!
#endif // MG_PLATFORM_WINDOWS



//debug模式设置
#ifdef MG_ENABLE_ASSERTS
	#define MG_CLIENT_ASSERT(x, ...) { if(!(x)) { MG_CLIENT_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define MG_CORE_ASSERT(x, ...) { if(!(x)) { MG_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define MG_CLIENT_ASSERT(x, ...)
	#define MG_CORE_ASSERT(x, ...)
#endif // MG_ENABLE_ASSERTS




#define BIT(x) (1 << x)

#define MG_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
