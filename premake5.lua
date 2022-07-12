workspace "Ivy"
	configurations { "Debug", "Release" }
	platforms { "x64" }
	
	filter "configurations:Debug"
		defines { "DEBUG" }
	filter "configurations:Release"
		defines { "NDEBUG" }
	filter "platforms:x64"
		system "Windows"
        architecture "x86_64"
	
project "global"
	kind "StaticLib"
	language "C++"
	location "global"
	
	files { "global/**.h", "global/**.cpp" }
	
	pchheader "global.h"
	pchsource "global.cpp"

project "ivy"
	kind "SharedLib"
	language "C++"
	location "ivy"
	
	files { "ivy/**.h", "ivy/**.cpp" }

project "sandbox"
	kind "ConsoleApp"
	language "C++"
	location "sandbox"
	
	files { "sandbox/**.h", "sandbox/**.cpp" }

