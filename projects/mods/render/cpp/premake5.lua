------------------------------------------
-- Common
------------------------------------------

function commonGlewConfig()
	-- Windows-specific
	filter {"configurations:Debug", "system:windows"}
		defines {
			-- glew (static)
			"WIN32",
			"_LIB",
			"WIN32_LEAN_AND_MEAN",
			"GLEW_STATIC"
		}
	filter {"configurations:Release", "system:windows"}
		defines {
			-- glew (static)
			"WIN32",
			"NDEBUG",
			"_LIB",
			"WIN32_LEAN_AND_MEAN",
			"GLEW_STATIC"
		}
	filter {}
end

------------------------------------------
-- Rendering module
------------------------------------------

project "ModRender"
	commonModConfigCPP()
	dependson {
		"glew",
	}
	includedirs {
		"glew/include"
	}

	filesCPP {
		"*",
		"entities/**",
		"loaders/**",
		"bind/**"
	}
	excludes {
		-- Glew is in another project so we exclude it
		"glew/**"
	}
	-- Windows-specific
	filter {"system:windows"}
		filesCPP {
			"win32/*"
		}
	filter {}

	links {
		"glew",
		"opengl32",
	}
	filter "system:windows"
		links {
			"opengl32"
		}
	filter {}

	filter "configurations:Debug"
		objdir "_obj/debug"
	filter "configurations:Release"
		objdir "_obj/release"

	commonGlewConfig()


------------------------------------------
-- Glew
------------------------------------------

project "glew"
	kind "StaticLib"
	location "."
	targetdir ".."
	files {
		"glew/**.h",
		"glew/**.c",
	}
	links {
		"opengl32"
	}
	includedirs {
		"glew/include"
	}

	filter "configurations:Debug"
		objdir "_obj/glew_debug"
	filter "configurations:Release"
		objdir "_obj/glew_release"

	commonGlewConfig()
