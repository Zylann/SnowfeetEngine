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

	-- Walks througth folders to include compliant premake5 projects
	local fileList = os.matchfiles("./**premake5.lua")
	for k,fpath in pairs(fileList) do
		if fpath ~= "premake5.lua" and -- Ignore the current file
		   fpath:find("_old") == nil and  -- Ignore _old stuff
		   fpath:find("_draft") == nil -- Ignore tests
		then
			--print(fpath)
			dofile(fpath)
		end
	end


	-- OLD CODE

	-- include("json")
	-- include("angelscript")
	-- include("core")
	-- include("main")
	-- include("zlib")

	-- -- Modules
	-- include("projects/mods/render/cpp")
	-- include("projects/mods/image/cpp")

	-- -- Tools
	-- include("projects/tools/editor/cpp")	

	-- -- Samples
	-- include("projects/samples/nativehelloworld/cpp")

