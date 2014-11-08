solution "SnowfeetEngine"
	platforms { "x32" }
	--location "."

	-- Global default configurations
	configurations { "Debug", "Release" }
	includedirs {
		"angelscript",
		"json",
		"." -- core
	}
	filter "configurations:Debug"
		flags {
			"Symbols" -- Produce debug symbols
		}
	filter "configurations:Release"
		optimize "On"
		flags {
			"LinkTimeOptimization"
		}

	--------------------------------------------
	-- TODO automatic file detector

	-- Core
	include("json")
	include("angelscript")
	include("core")
	include("main")
	include("zlib")

	-- Modules
	include("projects/mods/render/cpp")
	include("projects/mods/entity/cpp")


