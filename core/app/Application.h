﻿//------------------------------------------------------------------------------
//        /\    /\                      _______   __    _   _______   _   _   _   ______   ______   ______   _______
//       /  |  /  \         ____       |  _____| |  \  | | |  ___  | | | | | | | |  ____| |  ____| |  ____| |__   __|
//      / /\|_/ /\ \       /___/\      | |_____  |   \ | | | |   | | | | | | | | | |____  | |____  | |____     | |
//     /            |          \ \     |_____  | | |\ \| | | |   | | | | | | | | |  ____| |  ____| |  ____|    | |
//    |   _    _    |           \ \     _____| | | | \   | | |___| | | |_| |_| | | |      | |____  | |____     | |
//    |  (_)  (_)   /           /  /   |_______| |_|  \__| |_______| |_________| |_|      |______| |______|    |_|
//  --- \        / ---         /  /     _______   __    _   _______   _   __    _   ______ 
//   --- \/\/\/\/ ---_________/  /     |  _____| |  \  | | |  _____| | | |  \  | | |  ____|
//      \________/              /      | |_____  |   \ | | | |  ___  | | |   \ | | | |____ 
//      |                      /       |  _____| | |\ \| | | | |_  | | | | |\ \| | |  ____|
//      \                      |       | |_____  | | \   | | |___| | | | | | \   | | |____ 
//       \        __________   |       |_______| |_|  \__| |_______| |_| |_|  \__| |______|
//       |  /\   /       \  \  |
//       |  ||  |        |  |  |       Independence is the key to freedom.
//       |__||__|        |__|__|       Meow.
//      /___//__/       /__//__/
//------------------------------------------------------------------------------

/*
Application.h
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_APPLICATION__
#define __HEADER_SN_APPLICATION__

#include <core/reflect/ObjectTypeDatabase.h>
#include <core/app/CommandLine.h>
#include <core/app/Project.h>
#include <core/app/Module.h>
#include <core/app/ScriptManager.h>
#include <core/app/TimeStepper.h>
#include <core/app/DriverManager.h>
#include <core/scene/Scene.h>
#include <core/system/SharedLib.h>
#include <map>

namespace sn
{

/// \brief Top-level class handling the execution of the program.
/// There must be only one Application instance created in the
/// whole program execution.
class SN_API Application : public NonCopyable
{
public:

    Application();
    ~Application();

    /// \brief Gets the singleton
	static Application & get();

    /// \brief Executes the application. Cannot be called twice on the same Application.
    /// \note This is already done if you use the appmain() function.
    int execute(CommandLine commandLine);

    /// \brief Gets the absolute path to the projects folder
    inline String getPathToProjects() const { return m_pathToProjects; }

    /// \brief Gets the main script manager
    inline ScriptManager & getScriptManager() { return m_scriptEngine; }

    /// \brief Gets the drivers
    const DriverManager & getDriverManager() const { return m_drivers; }

    /// \brief Sets the running flag to false in order to exit the application
    /// at the end of the current update.
    void quit();

    bool loadModule(const std::string & name);

    // TODO Move SystemGUI here as a member variable, and let Application be the only one singleton?

private:

    /// \brief Prints a quick how-to use for launching the application from a command line
    void printCommandLineUsage();

    /// \brief Parses the given command line object and configures the application accordingly
    bool parseCommandLine(CommandLine commandLine);

    void unloadAllModules();

    /// \brief Loads a project from its path within one of the roots known by the engine.
    Project * loadProject(const String & path);

    void unloadAllProjects();

    /// \brief Executes the application without catching any exception (no try/catch)
    int executeEx();

    /// \brief Updates the main thread for a specified amount of time (main loop)
    void update(Time delta);

private:
    static Application * g_applicationInstance;

private:
    //------------------------------------
    // Attributes
    //------------------------------------

    // TODO Rename m_scriptManager
    /// \brief Main script manager.
    ScriptManager m_scriptEngine;

    /// \brief Top-level main scene
    Scene * m_scene;

    /// \brief Drivers
    DriverManager m_drivers;

    /// \brief Main root directory for all paths used in projects
    String m_pathToProjects;

    /// \brief Path (relative to the root) to the main module to be executed
    String m_pathToMainProject;

    // TODO Maybe we don't need this in the core, only graphics or physics simulation really need stable frequency.
    TimeStepper m_timeStepper;

    /// \brief [directory] => Project
    std::map<String, Project*> m_projects;

    /// \brief [name] => Module
    std::unordered_map<std::string, Module*> m_modules;

    /// \brief The main loop will run as long as this flag is true
    bool m_runFlag;
    
    bool m_dumpProfilingOnClose;

};

} // namespace sn

#endif // __HEADER_SN_APPLICATION__

