------------------------------------------
-- Assimp library binding for Snowfeet Engine
project "ModAssimp"
    platforms { "x32" }
    commonModConfigCPP()
    files {
        "*.h",
        "*.cpp"
    }
    links {
        "LibAssimp"
    }
    dependson {
        "LibAssimp"
    }
    filter "configurations:Debug"
        objdir "_obj/debug"
    filter "configurations:Release"
        objdir "_obj/release"
		

------------------------------------------
project "LibAssimp"
	platforms { "x32" }
	kind "StaticLib"
	language "C++"
	location "."
	targetdir ".."

	dependson {
		"zlib"
	}

	links {
		"zlib"
	}

    defines {
    	
    	-- I use the zlib from SnowfeetEngine because the one included in Assimp
    	-- cannot be easily compiled with premake (because of generated files)
    	"ASSIMP_BUILD_NO_OWN_ZLIB",

        -----------------------------------
        "_CRT_SECURE_NO_WARNINGS",
        "_SCL_SECURE_NO_WARNINGS",
        -----------------------------------
        "ASSIMP_BUILD_BOOST_WORKAROUND",
        "ASSIMP_BUILD_NO_X_IMPORTER",
        "ASSIMP_BUILD_NO_3DS_IMPORTER",
        "ASSIMP_BUILD_NO_MD3_IMPORTER",
        "ASSIMP_BUILD_NO_MDL_IMPORTER",
        "ASSIMP_BUILD_NO_MD2_IMPORTER",
        -- "ASSIMP_BUILD_NO_PLY_IMPORTER",
        "ASSIMP_BUILD_NO_ASE_IMPORTER",
        -- "ASSIMP_BUILD_NO_OBJ_IMPORTER",
        "ASSIMP_BUILD_NO_HMP_IMPORTER",
        "ASSIMP_BUILD_NO_SMD_IMPORTER",
        "ASSIMP_BUILD_NO_MDC_IMPORTER",
        "ASSIMP_BUILD_NO_MD5_IMPORTER",
        "ASSIMP_BUILD_NO_STL_IMPORTER",
        "ASSIMP_BUILD_NO_LWO_IMPORTER",
        "ASSIMP_BUILD_NO_DXF_IMPORTER",
        "ASSIMP_BUILD_NO_NFF_IMPORTER",
        "ASSIMP_BUILD_NO_RAW_IMPORTER",
        "ASSIMP_BUILD_NO_OFF_IMPORTER",
        "ASSIMP_BUILD_NO_AC_IMPORTER",
        "ASSIMP_BUILD_NO_BVH_IMPORTER",
        "ASSIMP_BUILD_NO_IRRMESH_IMPORTER",
        "ASSIMP_BUILD_NO_IRR_IMPORTER",
        "ASSIMP_BUILD_NO_Q3D_IMPORTER",
        "ASSIMP_BUILD_NO_B3D_IMPORTER",
        "ASSIMP_BUILD_NO_COLLADA_IMPORTER",
        "ASSIMP_BUILD_NO_TERRAGEN_IMPORTER",
        "ASSIMP_BUILD_NO_CSM_IMPORTER",
        "ASSIMP_BUILD_NO_3D_IMPORTER",
        "ASSIMP_BUILD_NO_LWS_IMPORTER",
        "ASSIMP_BUILD_NO_OGRE_IMPORTER",
        "ASSIMP_BUILD_NO_MS3D_IMPORTER",
        "ASSIMP_BUILD_NO_COB_IMPORTER",
        "ASSIMP_BUILD_NO_BLEND_IMPORTER",
        "ASSIMP_BUILD_NO_Q3BSP_IMPORTER",
        "ASSIMP_BUILD_NO_NDO_IMPORTER",
        "ASSIMP_BUILD_NO_IFC_IMPORTER",
        "ASSIMP_BUILD_NO_M3_IMPORTER",
        "ASSIMP_BUILD_NO_XGL_IMPORTER"
    }
    files {
        "assimp/code/**h",
        "assimp/code/**.cpp",
        "assimp/contrib/ConvertUTF/*.h",
        "assimp/contrib/ConvertUTF/*.cpp",
        "assimp/include/**.h",
        "assimp/include/**.hpp",
        "assimp/include/**.inl"
    }
    includedirs {
        "assimp/code/BoostWorkaround",
        "assimp/code",
        "assimp/",
        SnowfeetRoot.."/zlib"
    }

    -- configuration "not xcode*"
    --     pchheader ("AssimpPCH.h")
    --     pchsource ("code/AssimpPCH.cpp")

    -- Visual Studio only
    linkoptions {
    	-- Ignore empty obj file errors (LNK4221).
    	-- This happens because Assimp disables loaders by enclosing .cpp files within #ifdef blocks,
    	-- making entire files to compile without any symbols.
    	"/ignore:4221"
	}

    configuration "Debug"
		objdir "_obj/libassimp/debug"
        flags { "Symbols" }
        defines { "DEBUG" }
           
    configuration "Release"
		objdir "_obj/libassimp/release"
        defines { "NDEBUG", "_NDEBUG" }
        optimize "Full"

		
