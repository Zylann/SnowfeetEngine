project "ModOculus"
    commonModConfigCPP()
    files {
        "*.h",
        "**.hpp",
        "**.cpp",
    }
    includedirs {
        "../external/LibOVR/Include",
        "../external/LibOVR/Src" -- LOL
    }
    links {
        "ws2_32",
        "winmm"
    }
    filter "configurations:Debug"
        objdir "_obj/debug"
        links {
            "../external/LibOVR/Lib/Win32/VS2013/libovrd.lib"
        }
    filter "configurations:Release"
        objdir "_obj/release"
        links {
            "../external/LibOVR/Lib/Win32/VS2013/libovr.lib"
        }

