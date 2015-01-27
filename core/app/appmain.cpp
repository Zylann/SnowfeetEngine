#include "appmain.hpp"
#include "Application.hpp"
#include "../system/console/console.hpp"

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
    // Maintains the console window open on exit
    std::cout << "I: Execution finished with code " << exitCode << ". Press a key to dismiss..." << std::endl;
    sn::pauseConsole();
#endif

    return exitCode;
}

} // namespace sn


