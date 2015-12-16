project "Squirrel"
	kind "StaticLib"
	language "C++"
	location "." -- what the fuck premake5
	includedirs {
		"."
	}
	files {
		"**.h",
		"**.cpp"
	}

	filter "configurations:Debug"
		targetdir "../_bin/debug"
		objdir "../_obj/debug"
	filter "configurations:Release"
		targetdir "../_bin/release"
		objdir "../_obj/release"

	-- Visual Studio only
	filter "system:windows"
		defines {
			-- Override to allow modules to access Squirrel functions
			"SQUIRREL_API=__declspec(dllexport)",
			-- Disable deprecation of C functions used by Squirrel
			"_CRT_SECURE_NO_WARNINGS"
		}
