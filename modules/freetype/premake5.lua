project "ModFreetype"
    commonModConfigCPP()
    files {
        "*.hpp",
        "*.cpp"
    }
    includedirs {
        "freetype/include"
    }
    links {
        "LibFreetype"
    }
    -- filter "configurations:Debug"
    --     objdir "_obj/debug"
    -- filter "configurations:Release"
    --     objdir "_obj/release"

--------------------------------------------------
project "LibFreetype"
    kind "StaticLib"
    location "."
    targetdir ".."

    includedirs {
        "freetype/include"
    }

    -- Note: for some reason Freetype files we have to include or not aren't separated,
    -- it's difficult to write a rule that includes the good ones automagically.
    -- So they are specified here one by one.
    -- Source of this list: Visual 2010 project shipped with the library

    files {
        -- Headers

        "freetype/include/ft2build.h",
        "freetype/include/config/ft2build.h",
        "freetype/include/config/ftheader.h",
        "freetype/include/config/ftmodule.h",
        "freetype/include/config/ftoption.h",
        "freetype/include/config/ftstdlib.h",

        -- Source

        "freetype/src/base/ftbase.c",
        "freetype/src/base/ftbitmap.c",
        "freetype/src/base/ftfstype.c",
        "freetype/src/base/ftgasp.c",
        "freetype/src/base/ftglyph.c",
        "freetype/src/base/ftinit.c",
        "freetype/src/base/ftstroke.c",
        "freetype/src/base/ftsystem.c",

        "freetype/src/autofit/autofit.c",
        "freetype/src/bdf/bdf.c",
        "freetype/src/cff/cff.c",
        "freetype/src/cache/ftcache.c",
        "freetype/src/gzip/ftgzip.c",
        "freetype/src/lzw/ftlzw.c",
        "freetype/src/smooth/smooth.c",

        -- Modules

        "freetype/src/base/ftbbox.c",
        "freetype/src/base/ftgxval.c",
        "freetype/src/base/ftlcdfil.c",
        "freetype/src/base/ftmm.c",
        "freetype/src/base/ftotval.c",
        "freetype/src/base/ftpatent.c",
        "freetype/src/base/ftpfr.c",
        "freetype/src/base/ftsynth.c",
        "freetype/src/base/fttype1.c",
        "freetype/src/base/ftwinfnt.c",
        "freetype/src/base/ftxf86.c",

        "freetype/src/pcf/pcf.c",
        "freetype/src/pfr/pfr.c",
        "freetype/src/psaux/psaux.c",
        "freetype/src/pshinter/pshinter.c",
        "freetype/src/psnames/psmodule.c",
        "freetype/src/raster/raster.c",
        "freetype/src/sfnt/sfnt.c",
        "freetype/src/truetype/truetype.c",
        "freetype/src/type1/type1.c",
        "freetype/src/cid/type1cid.c",
        "freetype/src/type42/type42.c",
        "freetype/src/winfonts/winfnt.c",
    }


    --[[
    files {
        "freetype/**.c",
        "freetype/**.h"
    }

    excludes {
        -- Exclude Mac-only files
        "freetype/src/base/ftmac.c",

        -- Only include these if FT_OPTION_AUTOFIT2 is defined
        "freetype/src/autofit/aflatin2.c",
        "freetype/src/autofit/aflatin2.h",

        -- Exclude tools
        "freetype/src/tools/**"
    }
    --]]

    modObjDir("freetype")

    filter "configurations:Debug"
        flags { "Symbols" }
        defines {
            "_DEBUG",
            "_LIB",
            "FT_DEBUG_LEVEL_ERROR",
            "FT_DEBUG_LEVEL_TRACE",
            "FT2_BUILD_LIBRARY"
        }
        files {
            "freetype/builds/windows/ftdebug.c",
        }

    filter "configurations:Release"
        optimize "Full"
        defines {
            "NDEBUG",
            "FT2_BUILD_LIBRARY"
        }

    -- Windows-specific
    filter {"system:windows"}
        defines {
            "WIN32",
            "_CRT_SECURE_NO_WARNINGS",
        }
    filter {"configurations:Debug", "system:windows" }
        files {
            "freetype/builds/windows/ftdebug.c",
        }
    --filter {"configurations:Release", "system:windows"}
