project "JsonBox"
	platforms { "x32" }
	kind "StaticLib"
	language "C++"
	location "." -- what the fuck premake5
	files {
		"JsonBox/*.h",
		"JsonBox/*.cpp",
		"JsonBox.h"
	}
	-- includedirs {
	-- 	"."
	-- }
	filter "configurations:Debug"
		targetdir "../_bin/debug"
		objdir "../_obj/debug"
	filter "configurations:Release"
		targetdir "../_bin/release"
		objdir "../_obj/release"


