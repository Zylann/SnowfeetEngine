/*
Application.cpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include "Application.hpp"
#include "Module.hpp"
#include "../util/Log.hpp"
#include "../system/thread/Thread.hpp"
#include "../system/console/console.hpp"
#include "../object_types.hpp"
#include "../system/gui/SystemGUI.hpp"
#include "../asset/AssetDatabase.hpp"

namespace sn
{

//------------------------------------------------------------------------------
Application * Application::g_applicationInstance = nullptr;

//------------------------------------------------------------------------------
Application & Application::get()
{
    return *g_applicationInstance;
}

//------------------------------------------------------------------------------
Application::Application() :
    m_scriptEngine(*this),
    m_scene(nullptr),
    m_runFlag(false)
{
    SN_ASSERT(g_applicationInstance == nullptr, "Application: multiple instances are not allowed.");
    g_applicationInstance = this;
}

//------------------------------------------------------------------------------
Application::~Application()
{
    // TODO Unload modules in right order!
	// Unload modules
    for (auto it = m_modules.begin(); it != m_modules.end(); ++it)
    {
        delete it->second;
    }
}

//------------------------------------------------------------------------------
int Application::execute(CommandLine commandLine)
{
    sn::Log::get().openFile("snowfeet_log.txt");

    SN_LOG("Enter execute()");

    int exitCode = 0;

    try
    {
        if (parseCommandLine(commandLine))
        {
            exitCode = executeEx();
        }
    }
    catch (std::exception & e)
    {
        SN_ERROR("Fatal exception: " << e.what());
        exitCode = -1;
    }

    SN_LOG("Exit execute()");

    return exitCode;
}

//------------------------------------------------------------------------------
int Application::executeEx()
{
    if (m_pathToMainMod.empty())
    {
        SN_WARNING("No main mod specified.");
        return 0;
    }

    // Initialize global random seed
    math::randomSeed();

    registerObjectTypes(ObjectTypeDatabase::get());

    m_scriptEngine.initialize();

    m_scene = new Scene();

	// Initialize AssetDatabase
	AssetDatabase::get().setRoot(m_pathToProjects);

    // Consider the app to be running from now
    m_runFlag = true;

    // Corelib is the only module to always be loaded
    loadModule(L"mods/corelib/corelib.mod.json");

    // Load the main module
    Module * mainModule = loadModule(m_pathToMainMod);

    // If a startup scene has been specified
    const ModuleInfo & mainModuleInfo = mainModule->getInfo();
    if (!mainModuleInfo.startupScene.empty())
    {
        // Load the scene
        String filePath = m_pathToProjects + L"/" + mainModuleInfo.directory + L"/" + mainModuleInfo.startupScene;
        m_scene->loadFromFile(toString(filePath), SerializationContext(mainModuleInfo.name));
    }

    // Configure time stepper (at last, to minimize the "startup lag")
    m_timeStepper.setDeltaRange(Time::seconds(1.f / 70.f), Time::seconds(1.f / 30.f));

    if (m_scene->getQuitFlag())
        m_runFlag = false;
    if (m_scene->getChildCount() == 0)
    {
        SN_DLOG("The scene is empty, the application will quit");
        m_runFlag = false;
    }

    SN_LOG("Entering main loop");

    // Enter the main loop
    while (m_runFlag)
    {
        Clock frameClock;
        m_timeStepper.onBeginFrame();

        // Process system GUI messages
        SystemGUI::processEvents();

        Event ev;
        while (SystemGUI::get().popEvent(ev))
        {
            // Forward event to the scene
            if (!m_scene->onSystemEvent(ev))
            {
                // If the event has not been handled, perform default actions
                if (ev.type == SN_EVENT_WINDOW_CLOSED)
                    quit();
            }
        }

        std::vector<Time> deltas = m_timeStepper.getCallDeltas();
        for (u32 i = 0; i < deltas.size() && m_runFlag; ++i)
        {
            // Call static update callbacks
            // Note: if quit() is called in one of these callbacks,
            // the next callbacks will not be executed.
            update(deltas[i]);
        }

        // TODO Sleeping here doesn't makes sense without a render context, so put this in RenderManager?
        // Sleep until the next frame
        Time sleepTime = m_timeStepper.getMinDelta() - frameClock.getElapsedTime();
        if (sleepTime.asMilliseconds() > 0)
        {
            Thread::sleep(sleepTime);
        }

        m_timeStepper.onEndFrame();
    }

    SN_LOG("Exiting main loop");

    // Destroy all entities but services (which may be needed for releasing assets)
    SN_LOG("Destroying entities...");
    m_scene->destroyChildrenButServices();

    // Release assets
    SN_LOG("Releasing assets...");
    AssetDatabase::get().releaseAssets();

    // Destroy all remaining entities
    SN_LOG("Destroying services...");
    m_scene->destroyChildren();
    if (m_scene->getRefCount() > 1)
        SN_ERROR("Scene is leaking " << (m_scene->getRefCount() - 1) << " times");

    // Destroy scene itself
    m_scene->release();

    // Close all windows
    SystemGUI::get().destroyAllWindows();

    // TODO uninitialize all scripts before modules get destroyed

    return 0;
}

//------------------------------------------------------------------------------
//void Application::callVoidCallback(const std::string & cbName)
//{
//    for (auto it = m_modules.begin(); it != m_modules.end(); ++it)
//    {
//        Module & m = *(it->second);
//        m.callVoidCallback(cbName);
//    }
//}

//------------------------------------------------------------------------------
void Application::update(Time delta)
{
    if (m_scene)
    {
        m_scene->update(delta);
        if (m_scene->getQuitFlag())
            m_runFlag = false;
    }
    //for (auto it = m_modules.begin(); it != m_modules.end(); ++it)
    //{
    //    Module & m = *(it->second);
    //    m.onUpdate(delta);
    //}
}

//------------------------------------------------------------------------------
bool Application::parseCommandLine(CommandLine commandLine)
{
    using namespace std;

    u32 argc = commandLine.getArgCount();

    if (argc == 1)
    {
        commandLine.addFromFile("commandline.txt");
        argc = commandLine.getArgCount();
    }

    SN_WLOG("Command line: " << commandLine.toWideString());

    if (argc == 5)
    {
        m_pathToProjects = L"projects";

        for (u32 i = 1; i < argc; ++i)
        {
            String arg = commandLine.getArg(i);
            if (arg == L"-p")
            {
                m_pathToProjects = commandLine.getArg(++i);
            }
            else if (arg == L"-x")
            {
                m_pathToMainMod = commandLine.getArg(++i);
            }
            else
            {
                SN_WERROR("Unrecognized command line argument: \"" << arg << '"' << endl);
                printCommandLineUsage();
                return false;
            }
        }

        return true;
    }
    else
    {
        printCommandLineUsage();
    }

    return false;
}

//------------------------------------------------------------------------------
void Application::printCommandLineUsage()
{
    std::cout << "Usage: SnowfeetApp.exe [-p <pathToProjects>] -x <pathToExecutableMod>" << std::endl;
    std::cout << "You can also use a commandline.txt file as input." << std::endl;
}

//------------------------------------------------------------------------------
Module * Application::loadModule(const String & path)
{
    Module * lastModule = nullptr;

    SN_WDLOG("Calculating dependencies to load module \"" << path << '"');

    std::list<ModuleInfo> modulesToLoad;
    Module::calculateDependencies(m_pathToProjects, path, modulesToLoad);
    
#ifdef SN_BUILD_DEBUG
    for (auto it = modulesToLoad.begin(); it != modulesToLoad.end(); ++it)
    {
        SN_WDLOG(L"> " + it->modFilePath);
    }
#endif

    // Load main module and all its dependencies
    for (auto it = modulesToLoad.begin(); it != modulesToLoad.end(); ++it)
    {
        const ModuleInfo & info = *it;
        if (m_modules.find(info.directory) != m_modules.end())
        {
            SN_WLOG("Module " << path << " is already loaded, skipping");
            continue;
        }

        SN_LOG("-------------");
        SN_WLOG("Loading module " << info.directory);

        Module * mod = nullptr;

        try
        {
            // Reset default namespace to avoid registering in a wrong namespace
            //m_scriptEngine.getEngine()->SetDefaultNamespace("");

            // Create Module object
            mod = new Module(*this, info);
            
            mod->loadNativeBindings(m_scriptEngine);
            mod->compileScripts();

            if (m_scene)
            {
                mod->createServices(*m_scene);
            }

            m_modules.insert(std::make_pair(info.directory, mod));
            lastModule = mod;
        }
        catch (std::exception & ex)
        {
            SN_ERROR("Failed to load module: " << ex.what());
            delete mod;
            throw ex;
            return false;
        }
    }

    SN_LOG("-------------");

    // Load Assets afterwards (so now all services assets would require should be available)
    for (auto it = modulesToLoad.begin(); it != modulesToLoad.end(); ++it)
    {
        // Load static assets
        const ModuleInfo & info = *it;
        AssetDatabase::get().loadAssets(info);
    }

    // Note: the last loaded module will be the one we requested when calling this function
    return lastModule; 
}

//------------------------------------------------------------------------------
void Application::quit()
{
    SN_LOG("Application: quit requested");
    m_runFlag = false;
}

//------------------------------------------------------------------------------
//void Application::compileScripts()
//{
//
//}


} // namespace sn



