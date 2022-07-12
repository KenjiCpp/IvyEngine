workspace "Ivy"
	configurations { "Debug", "Release" }
	platforms      { "x64" }
	
	filter "configurations:Debug"
		defines { "DEBUG" }
	filter "configurations:Release"
		defines { "NDEBUG" }
	filter "platforms:x64"
		system       "Windows"
        architecture "x86_64"
		
	startproject "sandbox"

project "ivy"
	kind        "SharedLib"
	language    "C++"
	location    "ivy"
	debugformat "c7"
	
	defines { "_DLL_BUILD" }
	files { "ivy/**.h", "ivy/**.cpp", "ivy/**.inl" }
	
	includedirs { "$(ProjectDir)include/ivy", "$(ProjectDir)" }
	
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
