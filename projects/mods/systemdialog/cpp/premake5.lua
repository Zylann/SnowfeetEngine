------------------------------------------
-- SystemDialog module
------------------------------------------

project "ModSystemDialog"
	commonModConfigCPP()
	filesCPP {
		"*",
		"bind/**"
	}
	-- Windows-specific
	filter {"system:windows"}
		filesCPP {
			"win32/*"
		}
	filter {}

	filter "configurations:Debug"
		objdir "_obj/debug"
	filter "configurations:Release"
		objdir "_obj/release"

