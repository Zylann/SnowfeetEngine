project "ModImage"
	platforms { "x32" }
	kind "SharedLib"
	language "C++"
	dependson { 
		"SnowfeetCore",
		--"libpng"
	}
	location "."
	targetdir ".."
	files {
		"*.h",
		"**.hpp",
		"**.cpp",
		"stbi/*.h"
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

------------------------------------------
-- Just in case we go back to libpng

-- project "libpng"
-- 	platforms { "x32" }
-- 	kind "StaticLib"
-- 	language "C"
-- 	dependson { 
-- 		"zlib"
-- 	}
-- 	location "."
-- 	targetdir ".."
-- 	files {
-- 		"png/*.h",
-- 		"png/*.c"
-- 	}
-- 	includedirs {
-- 		"../../../../zlib"
-- 	}
-- 	links {
-- 		"zlib"
-- 	}
-- 	filter "configurations:Debug"
-- 		objdir "_obj/debug"
-- 		defines {
-- 			"WIN32",
-- 			"_DEBUG",
-- 			"_WINDOWS"
-- 		}
-- 	filter "configurations:Release"
-- 		objdir "_obj/release"
-- 		defines {
-- 			"WIN32",
-- 			"NDEBUG",
-- 			"_WINDOWS"
-- 		}
