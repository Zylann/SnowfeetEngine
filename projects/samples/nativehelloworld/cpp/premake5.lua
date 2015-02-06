project "SampleNativeHello"
	platforms { "x32" }
	commonModConfigCPP()
	files {
		"**.hpp",
		"**.cpp",
	}
	filter "configurations:Debug"
		objdir "_obj/debug"
	filter "configurations:Release"
		objdir "_obj/release"
