project "ModImage"
	platforms { "x32" }
	commonModConfigCPP()
	files {
		"*.h",
		"**.hpp",
		"**.cpp",
		"stbi/*.h"
	}
	-- links {
	-- 	"libpng"
	-- }
	filter "configurations:Debug"
		objdir "_obj/debug"
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
