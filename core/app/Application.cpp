/*
Application.cpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include "Application.h"
#include "Module.h"
#include "../util/Log.h"
#include "../system/thread/Thread.h"
#include "../system/console/console.h"
#include "../object_types.h"
#include "../system/gui/SystemGUI.h"
#include "../asset/AssetDatabase.h"
#include "../util/Profiler.h"
#include "../scene/PackedEntity.h"
#include "../scene/PackedEntityLoader.h"

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
    m_runFlag(false),
    m_dumpProfilingOnClose(false)
{
    SN_ASSERT(g_applicationInstance == nullptr, "Application: multiple instances are not allowed.");
    g_applicationInstance = this;
}

//------------------------------------------------------------------------------
Application::~Application()
{
    unloadAllModules();
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
	SN_BEGIN_PROFILE_SAMPLE_NAMED("Startup");

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
    AssetDatabase::get().addLoader<PackedEntityLoader>();

    // Consider the app to be running from now
    m_runFlag = true;

    // Corelib is the only module to always be loaded
    loadModule(L"mods/corelib/corelib.mod.json");

    // Load the main module (including dependencies)
    Module * mainModule = loadModule(m_pathToMainMod);

    // If a startup scene has been specified
    const ModuleInfo & mainModuleInfo = mainModule->getInfo();
    if (!mainModuleInfo.startupScene.empty())
    {
        // Load the scene
        SerializationContext serializationContext(mainModuleInfo.name);
        PackedEntity * packedScene = AssetDatabase::get().getAsset<PackedEntity>(mainModuleInfo.name, mainModuleInfo.startupScene);
        if (packedScene)
        {
            packedScene->instantiate(*m_scene, serializationContext);
        }
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

	SN_END_PROFILE_SAMPLE();
    SN_LOG("Entering main loop");

#ifdef SN_BUILD_DEBUG
    bool printDebugInfo = false;
    AssetDatabase::get().printAssetList();
#endif

    // Enter the main loop
    while (m_runFlag)
    {
        Profiler::get().markFrame();
		SN_BEGIN_PROFILE_SAMPLE_NAMED("Poll system events");

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
                if (ev.type == SN_EVENT_WINDOW_ASKED_CLOSE)
                    quit();
            }
#ifdef SN_BUILD_DEBUG
            if (ev.type == SN_EVENT_KEY_DOWN && ev.keyboard.keyCode == SN_KEY_F3)
            {
                printDebugInfo = true;
            }
#endif
        }

        AssetDatabase::get().updateFileChanges();

		SN_END_PROFILE_SAMPLE();
		SN_BEGIN_PROFILE_SAMPLE_NAMED("Global update");

        std::vector<Time> deltas = m_timeStepper.getCallDeltas();
        //for (u32 i = 0; i < deltas.size() && m_runFlag; ++i)
        //{
        //    // Call static update callbacks
        //    // Note: if quit() is called in one of these callbacks,
        //    // the next callbacks will not be executed.
        //    update(deltas[i]);
        //}
        update(Time::milliseconds(16));

		SN_END_PROFILE_SAMPLE();
		SN_BEGIN_PROFILE_SAMPLE_NAMED("Global sleep");

        // TODO The update timing system should be improved
        // TODO Sleeping here doesn't makes sense without a render context, so put this in RenderManager?
        // Sleep until the next frame
        Time sleepTime = m_timeStepper.getMinDelta() - frameClock.getElapsedTime();
        if (sleepTime.asMilliseconds() > 0)
        {
            Thread::sleep(sleepTime);
        }

		SN_END_PROFILE_SAMPLE();

        m_timeStepper.onEndFrame();

#ifdef SN_BUILD_DEBUG
        if (printDebugInfo)
        {
            u32 fps = m_timeStepper.getRecordedFPS();
            std::stringstream ss;
            for (u32 i = 0; i < deltas.size(); ++i)
            {
                ss << " " << deltas[i].asMilliseconds();
            }
            SN_DLOG("Recorded FPS: " << fps << ", Deltas: " << ss.str() << ", sleepTime: " << sleepTime.asMilliseconds());
            printDebugInfo = false;
        }
        // Quit if no window allow us to do so... (useful to debug app close at the moment, might be removed in server apps)
        if (SystemGUI::get().getWindowCount() == 0)
        {
            SN_DLOG("[DEBUG] No window open, let's quit...");
            m_runFlag = false;
        }
#endif
    }

    SN_LOG("Exiting main loop");
	SN_BEGIN_PROFILE_SAMPLE_NAMED("Shutdown");

    // Destroy all entities but services (which may be needed for releasing assets)
    SN_LOG("Destroying entities...");
    m_scene->destroyChildrenButServices();

    // Release assets
    SN_LOG("Releasing assets...");
    AssetDatabase::get().setTrackFileChanges(false); // To ensure the watcher thread is closed
    AssetDatabase::get().releaseAssets();
    AssetDatabase::get().releaseLoaders();

    // Destroy all remaining entities
    SN_LOG("Destroying services...");
    m_scene->destroyChildren();
    if (m_scene->getRefCount() > 1)
        SN_ERROR("Scene is leaking " << (m_scene->getRefCount() - 1) << " times");

    // Destroy scene itself
    m_scene->release();

    // Destroy drivers
    SN_LOG("Releasing drivers...");
    m_drivers.unloadAllDrivers();

    // Close all windows
    SystemGUI::get().destroyAllWindows();

    // Uninitialize all scripts before modules get destroyed
    SN_LOG("Closing main Squirrel VM...");
    m_scriptEngine.close();

	SN_END_PROFILE_SAMPLE();

	if (Profiler::get().isEnabled() && m_dumpProfilingOnClose)
		Profiler::get().dump("profile_data.json", Profiler::DUMP_JSON);

#ifdef SN_BUILD_DEBUG
    u32 leakingObjects = Object::getInstanceCount();
    if (leakingObjects > 0)
    {
        SN_WARNING("Objects are leaking!");
        Object::printInstances();
    }
#endif

    // Unload all modules
    unloadAllModules();

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

    if (argc <= 1)
    {
        commandLine.addFromFile("commandline.txt");
        argc = commandLine.getArgCount();
    }

    SN_WLOG("Command line: " << commandLine.toWideString());

    // path + -p + value + -x + value = 5 args minimum
    if (argc >= 5)
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
            else if (arg == L"--trackfiles")
            {
                AssetDatabase::get().setTrackFileChanges(true);
            }
			else if (arg == L"--profile")
			{
				Profiler::get().setEnabled(true);
                if (i + 1 < argc && commandLine.getArg(i + 1) == L"dump")
                {
                    m_dumpProfilingOnClose = true;
                    ++i;
                }
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
    // TODO If the path is a directory, append mod file with directory's name

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

            m_drivers.loadDriversFromModule(info.name);

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
void Application::unloadAllModules()
{
    // TODO Unload modules in right order!
	// Unload modules
    for (auto it = m_modules.begin(); it != m_modules.end(); ++it)
    {
        delete it->second;
    }
    m_modules.clear();
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



