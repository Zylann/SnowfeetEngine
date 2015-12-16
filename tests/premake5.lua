project "Tests"
	kind "ConsoleApp"
	language "C++"
	includedirs {
		"../squirrel"
	}
	dependson {
		"SnowfeetCore"
	}
	location "."
	files {
		"**.h",
		"**.hpp",
		"**.cpp"
	}
	links {
		"SnowfeetCore"
	}
	filter "configurations:Debug"
		targetdir "../_bin/debug"
		objdir "_obj/debug"
	filter "configurations:Release"
		targetdir "../_bin/release"
		objdir "_obj/release"
