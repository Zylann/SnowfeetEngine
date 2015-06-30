project "ModTGUI"
    commonModConfigCPP()
    files {
        "**.h",
        "**.cpp"
    }
    filter "configurations:Debug"
        objdir "_obj/debug"
    filter "configurations:Release"
        objdir "_obj/release"
