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
		"JsonBox", -- TODO Don't link JsonBox, it's already done in the core (maybe a premake5 bug?)
		--"libpng"
	}
	filter "configurations:Debug"
		objdir "_obj/debug"
		defines {
			"SN_BUILD_DEBUG"
		}
	filter "configurations:Release"
		objdir "_obj/release"
