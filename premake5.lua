--------------------------------------------
-- Globals
--------------------------------------------

SnowfeetRoot = os.getcwd()

--------------------------------------------
-- Solution
--------------------------------------------

solution "SnowfeetEngine"
--workspace "SnowfeetEngine"
	-- Global default configurations
	configurations { "Debug", "Release" }
	characterset "MBCS" -- TODO Move the engine to Unicode!
	includedirs {
		"." -- repo source root
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

	-- Windows-specific
	filter "system:windows"
		architecture "x86"
		defines {
			"SN_PLATFORM_WINDOWS"
		}
	filter {}

	--------------------------------------------
	-- Helpers
	--------------------------------------------

	--! \brief Same as "files" command,
	--! but appends all C++ extensions to match files,
	--! so you don't have to specify all known C++ extensions.
	function filesCPP(patternList)
	    local t = {}
	    for i,v in ipairs(patternList) do
	        table.insert(t, v..".h")
	        table.insert(t, v..".hpp")
	        table.insert(t, v..".cpp")
	    end
	    files(t)
	end

	function commonModFilesCPP()
		-- Only files at top-level
	    filesCPP "*"

	    filter "system:windows"
			filesCPP "win32/*_win32"

		filter "system:linux"
			filesCPP "linux/*_linux"

		filter {}
		--...
	end

	function commonModLinks()
		links {
			"SnowfeetCore",
		}
	end

	function commonModIncludes()
		includedirs {
			SnowfeetRoot.."/squirrel"
		}
	end

	function commonModDefines()
		local modDir = path.getname(os.getcwd())
		local exportMacroName = string.upper(modDir)
		defines {
			"SN_"..exportMacroName.."_EXPORT"
		}
	end

	--! \brief Defines the location of output binary files for a module.
	function modTargetDir()
		filter "configurations:Debug"
			targetdir(SnowfeetRoot.."/_bin/debug")
		filter "configurations:Release"
			targetdir(SnowfeetRoot.."../_bin/release")
		filter {}
	end

	--! \brief Defines the location of intermediary build files for a module.
	--! \param subDir (optional string): indicates a sub-directory
	function modObjDir(subDir)
		local base = "./_obj/"..(subDir or "")
		filter "configurations:Debug"
			objdir(base.."debug")
		filter "configurations:Release"
			objdir(base.."release")
		filter {}
	end

	--! \brief Helper defining include directories and links to another module.
	--! \param t (table): list of module names
	function moduleDependencies(t)
		for i,moduleName in ipairs(t) do
			dependson { moduleName }
			-- Just write #include <modules/*>
			--includedirs { SnowfeetRoot.."/modules" }
			links { moduleName }
		end
	end

	function commonModConfigCPP()
		kind "SharedLib"
		language "C++"
		dependson { "SnowfeetCore" }

		-- Location of the project file
		location "."
		-- Location of the output binaries
		modTargetDir()

		commonModLinks()
		commonModIncludes()
		commonModDefines()
	end

	--------------------------------------------
	-- Core
	--------------------------------------------

	-- Core stuff
	--include("angelscript")
	include("squirrel")
	include("core")
	include("main")
	include("zlib")

	include("tests")

	--------------------------------------------
	-- Modules
	--------------------------------------------

	-- Include modules:
	-- Walks througth folders to include compliant premake5 projects
	local fileList = os.matchfiles("./modules/**premake5.lua")
	for k,fpath in pairs(fileList) do
		if fpath ~= "premake5.lua" and -- Ignore the current file
		   fpath:find("_old") == nil and  -- Ignore _old stuff
		   fpath:find("_draft") == nil -- Ignore tests
		then
			--print(fpath)
			include(fpath)
		end
	end
