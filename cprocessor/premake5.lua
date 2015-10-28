workspace "CProcessor"
	configurations {"Debug", "Release"}
	
	project "CProcessor"
		kind "ConsoleApp"
		language "C++"
		files {
			"*.h",
			"*.cpp"
		}
		filter "configurations:Debug"
			targetdir "_bin/debug"
			targetdir "_obj/debug"
		filter "configurations:Release"
			targetdir "_bin/release"
			targetdir "_obj/release"
