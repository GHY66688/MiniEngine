#pragma once

#ifdef MG_PLATFORM_WINDOWS
	#ifdef MG_BUILD_DLL
		#define MG_API __declspec(dllexport)
	#else 
		#define MG_API __declspec(dllimport)
	#endif // MG_BUILD_DLL
#else
	#error MiniEngine only support Windows!
#endif // MG_PLATFORM_WINDOWS
