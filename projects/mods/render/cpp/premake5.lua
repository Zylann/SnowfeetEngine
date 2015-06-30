------------------------------------------
-- Common

function commonGlewConfig()
	-- Windows-specific
	filter "configurations:Debug and system:windows"
		defines {
			-- glew (static)
			"WIN32",
			"_LIB",
			"WIN32_LEAN_AND_MEAN",
			"GLEW_STATIC"
		}
	filter "configurations:Release and system:windows"
		defines {
			-- glew (static)
			"WIN32",
			"NDEBUG",
			"_LIB",
			"WIN32_LEAN_AND_MEAN",
			"GLEW_STATIC"
		}
end

------------------------------------------
project "ModRender"
	commonModConfigCPP()
	dependson {
		"glew",
	}
	includedirs {
		"glew/include"
	}
	files {
		"**.h",
		"**.hpp",
		"**.cpp",
		-- TODO Move these to platform-specific filter
		"win32/*.hpp",
		"win32/*.cpp"
	}
	excludes {
		"glew/**"
	}
	links {
		"glew",
		"opengl32",
	}

	filter "configurations:Debug"
		objdir "_obj/debug"
	filter "configurations:Release"
		objdir "_obj/release"

	commonGlewConfig()


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
