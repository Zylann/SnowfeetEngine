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
	links {
		"Squirrel",
		"JsonBox",
		"ws2_32"
	}
	defines {
		-- To allow modules to use bindings from the core
		"SCRAT_EXPORT"
	}
	filter "configurations:Debug"
		targetdir "../_bin/debug"
		objdir "../_obj/debug"
	filter "configurations:Release"
		targetdir "../_bin/release"
		objdir "../_obj/release"


