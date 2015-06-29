/*
appmain.cpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include "appmain.h"
#include "Application.h"
#include "../system/console/console.h"

namespace sn
{

int appMain(int argc, char * argv[])
{
    int exitCode = 0;

    {
        std::cout << "Hello Universe!" << std::endl;
        Application app;
        exitCode = app.execute(CommandLine(argc, argv));
    }

#ifdef SN_BUILD_DEBUG

    // Report leaks
    //MemoryManager::get().reportLeaks();
    u32 leakingObjects = Object::getInstanceCount();
    std::cout << "D: Remaining objects: " << leakingObjects << std::endl;

    // Maintains the console window open on exit
    std::cout << "I: Execution finished with code " << exitCode << ". Press a key to dismiss..." << std::endl;
    sn::pauseConsole();

#endif

    return exitCode;
}

} // namespace sn


