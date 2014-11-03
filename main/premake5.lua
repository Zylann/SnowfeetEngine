project "SnowfeetMain"
	platforms { "x32" }
	kind "ConsoleApp"
	language "C++"
	dependson {
		"SnowfeetCore"
	}
	location "."
	files {
		"**.hpp",
		"**.cpp"
	}
	-- includedirs {
	-- 	"../angelscript", -- For AngelScript
	-- 	"../json", -- For JsonBox
	-- 	".." -- For the core to work as a library
	-- }
	links {
		"SnowfeetCore",
	}
	filter "configurations:Debug"
		targetdir "../_bin/debug"
		objdir "../_obj/debug"
		defines {
			"SN_DEBUG"
		}
	filter "configurations:Release"
		targetdir "../_bin/release"
		objdir "../_obj/release"

