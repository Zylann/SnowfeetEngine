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
	links {
		"SnowfeetCore",
	}
	filter "configurations:Debug"
		targetdir "../_bin/debug"
		objdir "../_obj/debug"
	filter "configurations:Release"
		targetdir "../_bin/release"
		objdir "../_obj/release"

