
workspace "CPPJSONParser"
	architecture "x64"
	configurations { "Debug", "Release" }
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "CPPJSONParser"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	targetdir("bin/" .. outputdir .. "%{prj.name}")
	objdir("bin-int/" .. outputdir .. "%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"src"
	}