#include <iostream>
//#include <Windows.h>
#include <core/app/Application.hpp>
#include <core/system/console/console.hpp>

int main(int argc, char * argv[])
{
    int returnCode = 0;

    {
        std::cout << "Hello Universe!" << std::endl;
        sn::Application app;
        returnCode = app.execute(sn::CommandLine(argc, argv));
    }

#ifdef SN_BUILD_DEBUG
    sn::pauseConsole();
#endif

    return returnCode;
}

//int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
//{
//	std::cout << "Hello Universe!" << std::endl;
//	sn::Application app;
//
//	int argc = 0;
//	LPWSTR * argv = CommandLineToArgvW(GetCommandLineW(), &argc);
//
//	return app.execute(sn::CommandLine(argc, argv));
//}



