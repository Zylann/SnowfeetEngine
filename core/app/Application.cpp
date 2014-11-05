#include "Application.hpp"
#include "../util/Log.hpp"
#include "Module.hpp"
#include "../system/thread/Thread.hpp"

namespace sn
{

//------------------------------------------------------------------------------
Application * g_instance = nullptr;

//------------------------------------------------------------------------------
Application & Application::get()
{
    return *g_instance;
}

//------------------------------------------------------------------------------
Application::Application() :
    m_scriptEngine(*this),
    m_runFlag(false)
{
    SN_ASSERT(g_instance == nullptr, "E: Application: multiple instances are not allowed.");
    g_instance = this;
}

//------------------------------------------------------------------------------
Application::~Application()
{
    for (auto it = m_modules.begin(); it != m_modules.end(); ++it)
    {
        delete it->second;
    }
}

//------------------------------------------------------------------------------
int Application::execute(CommandLine commandLine)
{
    sn::Log::get().openFile("log.txt");

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

#ifdef SN_DEBUG
    // Maintains the console window open on exit
    std::cout << "I: Execution finished with code " << exitCode << ". Press a key to dismiss..." << std::endl;
    getchar();
#endif

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

    m_scriptEngine.initialize();

    m_runFlag = true;

    // Corelib is the only module to always be loaded
    loadModule(L"mods/corelib/corelib.mod.json");

    // Load the main module
    Module * mainModule = loadModule(m_pathToMainMod);

    // Call onCreate
    callVoidCallback(CallbackName::CREATE);

    // Test presence of update function
    if (!mainModule->hasUpdateFunction())
    {
        SN_LOG("The main module has no " << CallbackName::UPDATE << " function, "
            "the application will not enter the main loop.");
        m_runFlag = false;
    }

    // Configure time stepper (at last, to minimize the "startup lag")
    m_timeStepper.setDeltaRange(Time::seconds(1.f / 70.f), Time::seconds(1.f / 30.f));

    if (m_runFlag)
    {
        // Call start callbacks
        callVoidCallback(CallbackName::START);
    }

    // Enter the main loop
    while (m_runFlag)
    {
        Clock frameClock;
        m_timeStepper.onBeginFrame();

        // TODO Event processing

        std::vector<Time> deltas = m_timeStepper.getCallDeltas();
        for (u32 i = 0; i < deltas.size() && m_runFlag; ++i)
        {
            // Call update callbacks
            // Note: if quit() is called in one of these callbacks,
            // the next callbacks will not be executed.
            update(deltas[i]);
        }

        // TODO Render call
        // Note: no render call will be effective if there is nothing to draw

        // Sleep until the next frame
        Time sleepTime = m_timeStepper.getMinDelta() - frameClock.getElapsedTime();
        if (sleepTime.asMilliseconds() > 0)
        {
            Thread::sleep(sleepTime);
        }

        m_timeStepper.onEndFrame();
    }

    // Call destroy callbacks
    callVoidCallback(CallbackName::DESTROY);

    return 0;
}

//------------------------------------------------------------------------------
void Application::callVoidCallback(const std::string & cbName)
{
    for (auto it = m_modules.begin(); it != m_modules.end(); ++it)
    {
        Module & m = *(it->second);
        m.callVoidCallback(cbName);
    }
}

//------------------------------------------------------------------------------
void Application::update(Time delta)
{
    for (auto it = m_modules.begin(); it != m_modules.end(); ++it)
    {
        Module & m = *(it->second);
        m.onUpdate(delta);
    }
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

    // Load main module and all its dependencies
    for (auto it = modulesToLoad.begin(); it != modulesToLoad.end(); ++it)
    {
        const ModuleInfo & info = *it;
        if (m_modules.find(info.directory) != m_modules.end())
        {
            SN_WLOG("Module " << path << " is already loaded, skipping");
            continue;
        }

        SN_WLOG("Loading module " << info.directory);

        Module * mod = nullptr;

        try
        {
            mod = new Module(*this, info);
            mod->loadAssets();
            mod->compileScripts();
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



