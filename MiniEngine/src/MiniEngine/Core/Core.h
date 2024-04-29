#pragma once

#include <memory>

//设置平台
#ifdef MG_PLATFORM_WINDOWS
	#if HZ_DYNAMIC_LINK
		#ifdef MG_BUILD_DLL
			#define MG_API __declspec(dllexport)
		#else 
			#define MG_API __declspec(dllimport)
		#endif // MG_BUILD_DLL
	#else
		#define MG_API
	#endif	// HZ_DYNAMIC_LINK
#else
	#error MiniEngine only support Windows!
#endif // MG_PLATFORM_WINDOWS



#ifdef MG_DEBUG
#define MG_ENABLE_ASSERTS
#endif // MG_DEBUG


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

namespace MG {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}
