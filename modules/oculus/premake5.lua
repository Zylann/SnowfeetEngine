project "ModOculus"
    -- TODO Don't build this module outside of Windows because it's not supported
    commonModConfigCPP()
    moduleDependencies {
        "ModRender"
    }
    files {
        "*.h",
        "*.hpp",
        "*.cpp",
    }
    includedirs {
        "external/LibOVR/Include",
        "external/LibOVR/Src" -- LOL
    }

    -- filter "configurations:Debug"
    --     objdir "_obj/debug"
    -- filter "configurations:Release"
    --     objdir "_obj/release"

    -- Windows-specific
    filter "system:windows"
        links {
            "ws2_32",
            "winmm"
        }

    -- Windows-specific
    filter {"configurations:Debug", "system:windows"}
        links {
            "external/LibOVR/Lib/Win32/VS2013/libovrd.lib"
        }
    filter {"configurations:Release", "system:windows"}
        links {
            "external/LibOVR/Lib/Win32/VS2013/libovr.lib"
        }
