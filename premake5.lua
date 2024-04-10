workspace "MiniEngine"
	architecture "x64"
	startproject "SandBox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "MiniEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "MiniEngine/vendor/Glad/include"
IncludeDir["ImGui"] = "MiniEngine/vendor/imgui"
IncludeDir["glm"] = "MiniEngine/vendor/glm"

include "MiniEngine/vendor/GLFW"
include "MiniEngine/vendor/Glad"
include "MiniEngine/vendor/imgui"



project "MiniEngine"
	location "MiniEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"



	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "MGpch.h"
	pchsource "MiniEngine/src/MGpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"

	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"MG_PLATFORM_WINDOWS",
			"MG_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
			"_CRT_SECURE_NO_WARNINGS"
		}


	filter "configurations:Debug"
		defines "MG_DEBUG"
		runtime "Debug"
		symbols "on"
		
	filter "configurations:Release"
		defines "MG_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "MG_DIST"
		runtime "Release"
		optimize "on"
	

project "SandBox"
	location "SandBox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"MiniEngine/vendor/spdlog/include",
		"MiniEngine/src",
		"MiniEngine/vendor",
		"%{IncludeDir.glm}"

	}

	links
	{
		"MiniEngine"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"MG_PLATFORM_WINDOWS"
		}


	filter "configurations:Debug"
		defines "MG_DEBUG"
		runtime "Debug"
		symbols "on"
		
	filter "configurations:Release"
		defines "MG_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "MG_DIST"
		runtime "Release"
		optimize "on"