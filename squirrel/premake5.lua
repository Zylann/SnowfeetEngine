project "Squirrel"
	platforms { "x32" }
	kind "StaticLib"
	language "C++"
	location "." -- what the fuck premake5
	files {
		"**.h",
		"**.cpp"
	}
	-- Visual Studio only
	defines {
		-- Override to allow modules to access Squirrel functions
		"SQUIRREL_API=__declspec(dllexport)",
		-- Disable deprecation of C functions used by Squirrel
		"_CRT_SECURE_NO_WARNINGS"
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
