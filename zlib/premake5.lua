project "zlib"
	platforms { "x32" }
	kind "SharedLib"
	language "C"
	location "."

	files {
		"*.h",
		"*.c"
	}

	filter "configurations:Debug"
		targetdir "../_bin/debug"
		objdir "../_obj/debug"

	filter "configurations:Release"
		targetdir "../_bin/release"
		objdir "../_obj/release"

	-- Windows-specific
	filter "configurations:Debug and system:windows"
		defines {
			"WIN32",
			"_CRT_NONSTDC_NO_DEPRECATE",
			"_CRT_SECURE_NO_DEPRECATE",
			"_CRT_NONSTDC_NO_WARNINGS",
			"ZLIB_DLL",
			"ZLIB_WINAPI",
			-- "ASMV",
			-- "ASMINF"
		}
	filter "configurations:Release and system:windows"
		defines {
			"WIN32",
			"_CRT_NONSTDC_NO_DEPRECATE",
			"_CRT_SECURE_NO_DEPRECATE",
			"_CRT_NONSTDC_NO_WARNINGS",
			"ZLIB_DLL",
			"ZLIB_WINAPI",
			-- "ASMV",
			-- "ASMINF"
		}
