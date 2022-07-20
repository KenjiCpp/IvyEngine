workspace "Ivy"
	configurations { "Debug", "Release" }
	platforms      { "x64" }
	
	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
	filter { }
	
	filter "platforms:x64"
		system       "Windows"
        architecture "x86_64"
	filter { }	
	
	startproject "sandbox"

project "ivy"
	kind        "SharedLib"
	language    "C++"
	location    "ivy"
	debugformat "c7"
	
	defines { "_DLL_BUILD" }
	files { "ivy/**.h", "ivy/**.cpp", "ivy/**.inl" }
	
	includedirs { "$(ProjectDir)include/ivy", "$(ProjectDir)", "$(VULKAN_SDK)/include" }
	links { "$(VULKAN_SDK)/lib/vulkan-1.lib" }

	targetdir "$(SolutionDir)bin/$(Configuration)-$(Platform)"
	objdir    "!$(SolutionDir)int/$(Configuration)-$(Platform)"
	
	pchheader "global.h"
	pchsource "ivy/global.cpp"
	forceincludes { "global.h" }
	
project "sandbox"
	kind        "ConsoleApp"
	language    "C++"
	location    "sandbox"
	debugformat "c7"
	
	files { "sandbox/**.h", "sandbox/**.cpp" }
	
	links { "ivy" }
	includedirs { "$(SolutionDir)ivy/include", "$(SolutionDir)ivy" }
	
	targetdir "$(SolutionDir)bin/$(Configuration)-$(Platform)"
	objdir    "!$(SolutionDir)int/$(Configuration)-$(Platform)"
