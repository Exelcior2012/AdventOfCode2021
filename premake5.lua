
workspace "AdventOfCode2021"
	configurations {"Debug", "Release"}
	platforms {"Win32", "Win64"}
	location "Build"

project "AdventOfCode2021"
	kind "ConsoleApp"

	language "C++"
	cppdialect "C++20"

	targetdir "Binaries/%{cfg.buildcfg}"

	rtti "Off"
	exceptionhandling "Off"
	warnings "High"
	
	disablewarnings { 
		"4100" -- Unreferenced formal parameter, no real downside
	}
	
	flags {"FatalWarnings"}
	conformancemode "On"

	files {
		"Include/**.h",
		"Source/**.h",
		"Source/**.cpp"
	}
	
	includedirs {
		"Include"
	}

	-- Reproduce the file structure inside the solution explorer in visual studio
	vpaths {
		["*"] = "**.*"
	}

	filter "configurations:Debug"
	defines { "DEBUG" }
	symbols "On"
	optimize "Off"

	filter "configurations:Release"
	defines {"NDEBUG", "RELEASE"}
	optimize "Speed"
