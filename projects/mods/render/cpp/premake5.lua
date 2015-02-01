------------------------------------------
project "ModRender"
	platforms { "x32" }
	commonModConfigCPP()
	dependson { 
		"glew"
	}
	includedirs {
		"glew/include"
	}
	files {
		"*.hpp",
		"*.cpp",
		-- TODO Move these to platform-specific filter
		"win32/*.hpp",
		"win32/*.cpp"
	}
	links {
		"glew",
		"opengl32"
	}
	filter "configurations:Debug"
		objdir "_obj/debug"
		defines {
			-- glew (static)
			"WIN32",
			"_LIB",
			"WIN32_LEAN_AND_MEAN",
			"GLEW_STATIC"
		}
	filter "configurations:Release"
		objdir "_obj/release"
		defines {
			-- glew (static)
			"WIN32",
			"NDEBUG",
			"_LIB",
			"WIN32_LEAN_AND_MEAN",
			"GLEW_STATIC"
		}


------------------------------------------
project "glew"
	platforms { "x32" }
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
		defines {
			"WIN32",
			"_LIB",
			"WIN32_LEAN_AND_MEAN",
			"GLEW_STATIC"
		}
	filter "configurations:Release"
		objdir "_obj/glew_release"
		defines {
			"WIN32",
			"NDEBUG",
			"_LIB",
			"WIN32_LEAN_AND_MEAN",
			"GLEW_STATIC"
		}

