--------------------------------------------
-- Solution
--------------------------------------------

solution "SnowfeetEngine"
	platforms { "x32" }
	--location "."

	-- Global default configurations
	configurations { "Debug", "Release" }
	includedirs {
		"squirrel",
		"json",
		"." -- core
	}
	filter "configurations:Debug"
		flags {
			"Symbols" -- Produce debug symbols
		}
		defines {
			"SN_BUILD_DEBUG"
		}
	filter "configurations:Release"
		optimize "On"
		flags {
			"LinkTimeOptimization"
		}

	--------------------------------------------
	-- Core
	--------------------------------------------

	-- Core stuff
	include("json")
	--include("angelscript")
	include("squirrel")
	include("core")
	include("main")
	include("zlib")

	--------------------------------------------
	-- Modules
	--------------------------------------------

	function commonLinks()
		links {
			"SnowfeetCore",

			-- Note: I don't think both should be there,
			-- because they already are statically linked to the core,
			-- which in turn is linked to modules...
			"JsonBox"
		}
	end

	function commonIncludes()
		--
	end

	function commonDefines()
		-- Modules bindings have to include core bindings
		-- if they want to interact with them
		defines {
			"SCRAT_IMPORT"
		}
	end

	function commonModConfigCPP()
		kind "SharedLib"
		language "C++"
		dependson { "SnowfeetCore" }
		location "."
		targetdir ".."
		commonLinks()
		commonIncludes()
		commonDefines()
	end

	-- Include modules:
	-- Walks througth folders to include compliant premake5 projects
	local fileList = os.matchfiles("./projects/**premake5.lua")
	for k,fpath in pairs(fileList) do
		if fpath ~= "premake5.lua" and -- Ignore the current file
		   fpath:find("_old") == nil and  -- Ignore _old stuff
		   fpath:find("_draft") == nil -- Ignore tests
		then
			--print(fpath)
			dofile(fpath)
		end
	end
