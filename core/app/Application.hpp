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

// Note: for a modular engine, I could have choosen a name like AtomEngine,
// PlanetEngine, LinkEngine or... aaah fuck, Snowfeet is good.
// Want to know what is a Snowfeet?
// It's a cat. A black cat with white paws and tail.
// They are the last species of cats in the future.
// They've become so rare they worth billions of credits, and...
// Are you still reading this? Go on!

#ifndef __HEADER_SN_APPLICATION__
#define __HEADER_SN_APPLICATION__

#include <core/reflect/ObjectTypeDatabase.hpp>
#include <core/app/CommandLine.hpp>
#include <core/app/Module.hpp>
#include <core/app/ScriptEngine.hpp>
#include <core/app/TimeStepper.hpp>
#include <core/scene/Scene.hpp>
#include <map>

namespace sn
{

/// \brief Top-level class handling the execution of the program.
/// There must be only one Application instance created in the
/// whole program execution.
class SN_API Application
{
public:

    Application();
    ~Application();

    int execute(CommandLine commandLine);

    // Gets the absolute path to the projects folder
    inline String getPathToProjects() const { return m_pathToProjects; }

    inline ScriptEngine & getScriptEngine() { return m_scriptEngine; }

    // Sets the running flag to false in order to exit the application
    // at the end of the current update.
    void quit();

    //static Application & get();

private:

    void printCommandLineUsage();
    bool parseCommandLine(CommandLine commandLine);
    Module * Application::loadModule(const String & path);
    int executeEx();
    //void compileScripts();

    void update(Time delta);
    //void callVoidCallback(const std::string & cbName);

    //------------------------------------
    // Attributes
    //------------------------------------

    ScriptEngine m_scriptEngine;

    // Top-level scene
    Scene * m_scene;

    String m_pathToMainMod;
    String m_pathToProjects;

    // TODO Maybe we don't need this in the core, only graphics or physics simulation really need stable frequency.
    TimeStepper m_timeStepper;

    // [directory] => Module
    std::map<String, Module*> m_modules;

    bool m_runFlag;

};

} // namespace sn

#endif // __HEADER_SN_APPLICATION__

