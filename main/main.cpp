#include <iostream>
#include <core/app/appmain.hpp>

int main(int argc, char * argv[])
{
    return sn::appMain(argc, argv);
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



