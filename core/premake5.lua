project "SnowfeetCore"
	platforms { "x32" }
	kind "SharedLib"
	language "C++"
	dependson {
		"AngelScript",
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
		"AngelScript",
		"JsonBox"
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


