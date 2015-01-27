project "Squirrel"
	platforms { "x32" }
	kind "StaticLib"
	language "C++"
	location "." -- what the fuck premake5
	files {
		"**.h",
		"**.cpp"
	}
	filter "configurations:Debug"
		targetdir "../_bin/debug"
		objdir "../_obj/debug"
		defines {
		}
	filter "configurations:Release"
		targetdir "../_bin/release"
		objdir "../_obj/release"
		defines {
		}
