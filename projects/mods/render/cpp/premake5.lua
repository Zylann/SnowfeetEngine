project "ModRender"
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
		"**.cpp"
	}
	links {
		"SnowfeetCore",
		"JsonBox"
	}
	filter "configurations:Debug"
		objdir "_obj/debug"
		defines {
			"SN_DEBUG"
		}
	filter "configurations:Release"
		objdir "_obj/release"

