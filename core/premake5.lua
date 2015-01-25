project "SnowfeetCore"
	platforms { "x32" }
	kind "SharedLib"
	language "C++"
	dependson {
		"Squirrel",
		"JsonBox"
	}
	location "."
	files {
		"**.h",
		"**.hpp",
		"**.cpp"
	}
	-- includedirs {
	-- 	"../angelscript", -- For AngelScript
	-- 	"../json", -- For JsonBox
	-- 	".." -- For the core to work as a library
	-- }
	links {
		"Squirrel",
		"JsonBox",
		"ws2_32"
	}
	defines {
		-- "AS_ADDON_SCRIPTTYPE_USE_STLSTRING=1"
	}
	filter "configurations:Debug"
		targetdir "../_bin/debug"
		objdir "../_obj/debug"
		defines {
			"SN_BUILD_DEBUG"
		}
	filter "configurations:Release"
		targetdir "../_bin/release"
		objdir "../_obj/release"


