---------------------------------------------------
-- Snowfeet Core
---------------------------------------------------

project "SnowfeetCore"

	kind "SharedLib"
	language "C++"
	dependson {
		"Squirrel"
	}
	location "."

	includedirs {
		"../squirrel"
	}

	---------------------------------------------------
	-- Files
	---------------------------------------------------

	filesCPP {
		"*",
		"app/**",
		"asset/**",
		"bind/**",
		"drivers/**",
		"sml/**",
		"math/**",
		"pcg/**",
		"reflect/**",
		"scene/**",
		"space/**",
		"squirrel/**",
		"util/**",

		"system/*"
	}
	-- Windows-specific
	filter "system:windows"
		filesCPP {
			"system/**_win32"
		}
		links {
			"winmm"
		}
	-- Linux-specific
	filter "system:linux"
		filesCPP {
			"system/**_linux"
		}
	filter {}

	---------------------------------------------------
	-- Links
	---------------------------------------------------

	links {
		"Squirrel",
	}
	-- Windows-specific
	filter "system:windows"
		links {
			-- winsock2 for network
			"ws2_32"
		}
	filter {}

	---------------------------------------------------
	-- Defines
	---------------------------------------------------

	defines {
		-- To export core API (by default SN_API is set to import)
		"SN_CORE_EXPORT"
	}

	---------------------------------------------------
	-- Compiler output
	---------------------------------------------------

	filter "configurations:Debug"
		targetdir "../_bin/debug"
		objdir "../_obj/debug"
	filter "configurations:Release"
		targetdir "../_bin/release"
		objdir "../_obj/release"
