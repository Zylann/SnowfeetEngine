project "ModImage"
	platforms { "x32" }
	kind "SharedLib"
	language "C++"
	dependson { 
		"SnowfeetCore",
		"libpng"
	}
	location "."
	targetdir ".."
	files {
		"**.hpp",
		"**.cpp"
	}
	links {
		"SnowfeetCore",
		--"JsonBox",
		"libpng"
	}
	filter "configurations:Debug"
		objdir "_obj/debug"
		defines {
			"SN_DEBUG"
		}
	filter "configurations:Release"
		objdir "_obj/release"

------------------------------------------
project "libpng"
	platforms { "x32" }
	kind "StaticLib"
	language "C"
	dependson { 
		"zlib"
	}
	location "."
	targetdir ".."
	files {
		"png/*.h",
		"png/*.c"
	}
	includedirs {
		"../../../../zlib"
	}
	links {
		"zlib"
	}
	filter "configurations:Debug"
		objdir "_obj/debug"
		defines {
			"WIN32",
			"_DEBUG",
			"_WINDOWS"
		}
	filter "configurations:Release"
		objdir "_obj/release"
		defines {
			"WIN32",
			"NDEBUG",
			"_WINDOWS"
		}
