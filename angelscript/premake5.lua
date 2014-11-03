project "AngelScript"
	platforms { "x32" }
	kind "StaticLib"
	language "C++"
	location "." -- what the fuck premake5
	files {
		"*.h",
		"*.cpp"
	}
	filter "configurations:Debug"
		targetdir "../_bin/debug"
		objdir "../_obj/debug"
		defines {
			"ANGELSCRIPT_EXPORT",
			"AS_DEBUG",
			"WIN32",
			"_DEBUG",
			"_LIB"
		}
	filter "configurations:Release"
		targetdir "../_bin/release"
		objdir "../_obj/release"
		defines {
			"ANGELSCRIPT_EXPORT",
			"WIN32",
			"_LIB"
		}
