project "Squirrel"
	platforms { "x32" }
	kind "StaticLib"
	language "C++"
	location "." -- what the fuck premake5
	files {
		"**.h",
		"**.cpp"
	}
	defines {
		-- Override to allow modules to access squirrel functions
		"SQUIRREL_API=__declspec(dllexport)"
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
