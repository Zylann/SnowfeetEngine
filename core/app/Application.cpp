/*
Application.cpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include <core/util/Log.h>
#include <core/system/filesystem.h>
#include <core/system/Thread.h>
#include <core/system/console.h>
#include <core/system/SystemGUI.h>
#include <core/system/Joystick.h>
#include <core/object_types.h>
#include <core/asset/AssetDatabase.h>
#include <core/util/Profiler.h>
#include <core/scene/PackedEntity.h>
#include <core/scene/PackedEntityLoader.h>

#include "Application.h"
#include "Module.h"

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

    if (m_pathToMainProject.empty())
    {
        SN_WARNING("No startup project specified.");
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

    // Corelib is the only dependency project to always be loaded
    loadProject(L"mods/corelib");

    // Load the main project (including dependencies)
    Project * mainProject = loadProject(m_pathToMainProject);

    // If a startup scene has been specified
    const ProjectInfo & mainProjectInfo = mainProject->getInfo();
    if (!mainProjectInfo.startupScene.empty())
    {
        // Initialize update layers
        // TODO UpdateManager config should be an asset
        m_scene->getUpdateManager().unserialize(mainProjectInfo.updateLayers);

        // Load the scene
        PackedEntity * packedScene = AssetDatabase::get().getAsset<PackedEntity>(mainProjectInfo.name, mainProjectInfo.startupScene);
        if (packedScene)
        {
            packedScene->instantiate(*m_scene, mainProjectInfo.name);
//#ifdef SN_BUILD_DEBUG
//            Entity::debugPrintEntityTree(*m_scene);
//#endif
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
        Joystick::updateJoysticks();

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

    unloadAllProjects();
    unloadAllModules();

    return 0;
}

//------------------------------------------------------------------------------
void Application::update(Time delta)
{
    if (m_scene)
    {
        m_scene->update(delta);
        if (m_scene->getQuitFlag())
            m_runFlag = false;
    }
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

    SN_DLOG("Working dir: " << getWorkingDirectory());
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
                m_pathToMainProject = commandLine.getArg(++i);
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
    std::cout << "Usage: SnowfeetApp.exe [-p <pathToProjectsDir>] -x <pathToStartupProjectDir>" << std::endl;
    std::cout << "Example: SnowfeetApp.exe -p ../../projects -x samples/rendertest" << std::endl;
    std::cout << "You can also use a commandline.txt file as input in your working directory." << std::endl;
}

//------------------------------------------------------------------------------
bool Application::loadModule(const std::string & fileName)
{
    SN_LOG("Loading shared lib \"" << fileName << "\"...");

    std::string name = getFileNameWithoutExtension(fileName);

    auto it = m_modules.find(name);
    if (it == m_modules.end())
    {
        // Load the code
        Module * mod = Module::loadFromFile(fileName);
        if (mod == nullptr)
            return false;

        auto & otb = ObjectTypeDatabase::get();

        // Call entry point
        otb.beginModule(mod->getName());
        mod->entryPoint({
            m_scriptEngine.getVM(),
            &otb
        });
        otb.endModule();

        // Register drivers
        m_drivers.loadDriversFromModule(name);

        // Register asset loaders
        AssetDatabase::get().addLoadersFromModule(name);

        m_modules[name] = mod;
        return true;
    }
    else
    {
        return true;
    }
}

//------------------------------------------------------------------------------
void Application::unloadAllModules()
{
    for (auto it = m_modules.begin(); it != m_modules.end(); ++it)
    {
        Module * mod = it->second;

        // Unload drivers
        m_drivers.unloadDriversFromModule(mod->getName());

        // Unload asset loaders
        AssetDatabase::get().releaseLoadersFromModule(mod->getName());

        ObjectTypeDatabase::get().unregisterModule(mod->getName());

        mod->release();
    }
    m_modules.clear();
}

//------------------------------------------------------------------------------
Project * Application::loadProject(const String & path)
{
    // TODO If the path is a directory, append mod file with directory's name

    Project * lastProject = nullptr;

    SN_WDLOG("Calculating dependencies to load project \"" << path << '"');

    std::list<ProjectInfo> projectsToLoad;
    Project::calculateProjectDependencies(m_pathToProjects, path, projectsToLoad);

#ifdef SN_BUILD_DEBUG
    for (auto it = projectsToLoad.begin(); it != projectsToLoad.end(); ++it)
    {
        SN_WDLOG(L"> " + it->filePath);
    }
#endif

	std::vector<Project*> projects;
	projects.reserve(projectsToLoad.size());

    // Load main module and all its dependencies
    for (auto it = projectsToLoad.begin(); it != projectsToLoad.end(); ++it)
    {
        const ProjectInfo & info = *it;
        if (m_projects.find(info.directory) != m_projects.end())
        {
            SN_WLOG("Project " << path << " is already loaded, skipping");
            continue;
        }

        SN_LOG("-------------");
        SN_WLOG("Loading project " << info.directory);

        Project * project = nullptr;

        try
        {
            // Reset default namespace to avoid registering in a wrong namespace
            //m_scriptEngine.getEngine()->SetDefaultNamespace("");

            // Create Module object
            project = new Project(*this, info);

            project->loadModules();
            project->compileScripts();

            m_projects.insert(std::make_pair(info.directory, project));
			projects.push_back(project);
            lastProject = project;
        }
        catch (std::exception & ex)
        {
            SN_ERROR("Failed to load project: " << ex.what());
            delete project;
            throw ex;
            return false;
        }
    }

    SN_ASSERT(m_scene != nullptr, "Scene is required");

    // Once modules are loaded, create component managers
    m_scene->getComponentSystem().createManagers();

	// Rebuild class mapping once all script classes have been defined
	m_scriptEngine.rebuildClassMapping();

    AssetDatabase & assetDatabase = AssetDatabase::get();

    // Load Assets afterwards (so now all modules assets would require should be available)
    for (auto it = projectsToLoad.begin(); it != projectsToLoad.end(); ++it)
    {
        SN_LOG("-------------");
        // Load static assets
        const ProjectInfo & info = *it;
        assetDatabase.loadAssets(info);
    }

    // Create scene managers
	size_t i = 0;
	for (auto it = projectsToLoad.begin(); it != projectsToLoad.end(); ++it)
	{
		const ProjectInfo & info = *it;
        sn::PackedEntity * pack = assetDatabase.getAsset<PackedEntity>(info.name, info.sceneManagers);
        if (pack)
        {
            // Instantiate managers
            std::vector<Entity*> rootEntities;
            pack->instantiate(*m_scene, info.name, &rootEntities);

            // Make them sticky (they won't go away when a new scene is loaded)
            for (u32 i = 0; i < rootEntities.size(); ++i)
            {
                Entity * e = rootEntities[i];
                e->setFlag(SN_EF_STICKY, true);
            }
        }
	}

    // Note: the last loaded module will be the one we requested when calling this function
    return lastProject;
}

//------------------------------------------------------------------------------
void Application::unloadAllProjects()
{
    for (auto it = m_projects.begin(); it != m_projects.end(); ++it)
    {
        delete it->second;
    }
    m_projects.clear();
}

//------------------------------------------------------------------------------
void Application::quit()
{
    SN_LOG("Application: quit requested");
    m_runFlag = false;
}

} // namespace sn
