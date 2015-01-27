project "SampleNativeHello"
	platforms { "x32" }
	kind "SharedLib"
	language "C++"
	dependson { 
		"SnowfeetCore"
	}
	location "."
	targetdir ".."
	files {
		"**.hpp",
		"**.cpp",
	}
	links {
		"SnowfeetCore",
	}
	filter "configurations:Debug"
		objdir "_obj/debug"
	filter "configurations:Release"
		objdir "_obj/release"
