------------------------------------------
project "ModRender"
	platforms { "x32" }
	commonModConfigCPP()
	dependson { 
		"glew",
		--"LibAssimp"
	}
	includedirs {
		"glew/include"
	}
	files {
		"*.h",
		"*.hpp",
		"*.cpp",
		"entities/*.hpp",
		"entities/*.cpp",
		"sq_binding/*.hpp",
		"sq_binding/*.cpp",
		"loaders/ply/*.hpp",
		"loaders/ply/*.cpp",
		"loaders/mshader/*.hpp",
		"loaders/mshader/*.cpp",
		
		-- TODO Move these to platform-specific filter
		"win32/*.hpp",
		"win32/*.cpp"
	}
	links {
		"glew",
		"opengl32",
		--"LibAssimp"
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

------------------------------------------
-- project "LibAssimp"
-- 	platforms { "x32" }
-- 	kind "StaticLib"
-- 	language "C++"
-- 	location "."
-- 	targetdir ".."

--     defines {
--         -----------------------------------
--         "_CRT_SECURE_NO_WARNINGS",
--         "_SCL_SECURE_NO_WARNINGS",
--         -----------------------------------
--         "ASSIMP_BUILD_BOOST_WORKAROUND",
--         "ASSIMP_BUILD_NO_X_IMPORTER",
--         "ASSIMP_BUILD_NO_3DS_IMPORTER",
--         "ASSIMP_BUILD_NO_MD3_IMPORTER",
--         "ASSIMP_BUILD_NO_MDL_IMPORTER",
--         "ASSIMP_BUILD_NO_MD2_IMPORTER",
--         -- "ASSIMP_BUILD_NO_PLY_IMPORTER",
--         "ASSIMP_BUILD_NO_ASE_IMPORTER",
--         -- "ASSIMP_BUILD_NO_OBJ_IMPORTER",
--         "ASSIMP_BUILD_NO_HMP_IMPORTER",
--         "ASSIMP_BUILD_NO_SMD_IMPORTER",
--         "ASSIMP_BUILD_NO_MDC_IMPORTER",
--         "ASSIMP_BUILD_NO_MD5_IMPORTER",
--         "ASSIMP_BUILD_NO_STL_IMPORTER",
--         "ASSIMP_BUILD_NO_LWO_IMPORTER",
--         "ASSIMP_BUILD_NO_DXF_IMPORTER",
--         "ASSIMP_BUILD_NO_NFF_IMPORTER",
--         "ASSIMP_BUILD_NO_RAW_IMPORTER",
--         "ASSIMP_BUILD_NO_OFF_IMPORTER",
--         "ASSIMP_BUILD_NO_AC_IMPORTER",
--         "ASSIMP_BUILD_NO_BVH_IMPORTER",
--         "ASSIMP_BUILD_NO_IRRMESH_IMPORTER",
--         "ASSIMP_BUILD_NO_IRR_IMPORTER",
--         "ASSIMP_BUILD_NO_Q3D_IMPORTER",
--         "ASSIMP_BUILD_NO_B3D_IMPORTER",
--         "ASSIMP_BUILD_NO_COLLADA_IMPORTER",
--         "ASSIMP_BUILD_NO_TERRAGEN_IMPORTER",
--         "ASSIMP_BUILD_NO_CSM_IMPORTER",
--         "ASSIMP_BUILD_NO_3D_IMPORTER",
--         "ASSIMP_BUILD_NO_LWS_IMPORTER",
--         "ASSIMP_BUILD_NO_OGRE_IMPORTER",
--         "ASSIMP_BUILD_NO_MS3D_IMPORTER",
--         "ASSIMP_BUILD_NO_COB_IMPORTER",
--         "ASSIMP_BUILD_NO_BLEND_IMPORTER",
--         "ASSIMP_BUILD_NO_Q3BSP_IMPORTER",
--         "ASSIMP_BUILD_NO_NDO_IMPORTER",
--         "ASSIMP_BUILD_NO_IFC_IMPORTER",
--         "ASSIMP_BUILD_NO_M3_IMPORTER",
--         "ASSIMP_BUILD_NO_XGL_IMPORTER"
--     }
--     files {
--         "assimp/code/**h",
--         "assimp/code/**.cpp",
--         "assimp/contrib/ConvertUTF/*.h",
--         "assimp/contrib/ConvertUTF/*.cpp",
--         "assimp/include/**.h",
--         "assimp/include/**.hpp",
--         "assimp/include/**.inl"
--     }
--     includedirs {
--         "assimp/code/BoostWorkaround",
--         "assimp/code"
--     }

--     -- configuration "not xcode*"
--     --     pchheader ("AssimpPCH.h")
--     --     pchsource ("code/AssimpPCH.cpp")

--     configuration "Debug"
-- 		objdir "_obj/libassimp/debug"
--         flags { "Symbols" }
--         defines { "DEBUG" }
           
--     configuration "Release"
-- 		objdir "_obj/libassimp/release"
--         defines { "NDEBUG", "_NDEBUG" }
--         optimize "Full"
