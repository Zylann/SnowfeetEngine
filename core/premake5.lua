project "SnowfeetCore"
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
	}
	-- Windows-specific
	filter "system:windows" {
		links {
			-- winsock2
			"ws2_32"
		}
	}
	filter {}

	defines {
		-- To allow modules to use bindings from the core
		"SCRAT_EXPORT",
		-- To export core API (by default SN_API is set to import)
		"SN_CORE_EXPORT"
	}

	filter "configurations:Debug"
		targetdir "../_bin/debug"
		objdir "../_obj/debug"
	filter "configurations:Release"
		targetdir "../_bin/release"
		objdir "../_obj/release"
