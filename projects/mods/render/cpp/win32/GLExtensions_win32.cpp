#include "../GLExtensions.hpp"
#include <core/util/Log.hpp>
#include <GL/glew.h>
#include <Windows.h>

namespace sn {
namespace render {

// Thanks to Megabyte Softworks, for its tutorial series on "from-scratch" OpenGL 3.3 
// http://www.mbsoftworks.sk/index.php?page=tutorials&series=1

//------------------------------------------------------------------------------
LRESULT CALLBACK MsgHandlerSimpleOpenGLClass(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	switch(uiMsg)
	{
		case WM_PAINT:
			BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;

		default:
			return DefWindowProc(hWnd, uiMsg, wParam, lParam); // Default window procedure
	}
	return 0;
}

//------------------------------------------------------------------------------
bool ensureGLExtensions()
{
	// This function cannot be called twice
	static bool initialized = false;
	if (initialized)
		return true;

    // Accessing OpenGL context creation above 2.0 requires an extension.
    // However, extensions are accessible only if we... created a context.
    // So the point of the code below is to create a dummy OpenGL context first,
    // initialize extensions and destroy this context. After that we'll have
    // access to what we want.

	HINSTANCE hInstance = GetModuleHandle(NULL);

	// Register fake window class
	WNDCLASSEX wc;

	const char * windowClassName = "SnowfeetFakeOpenGLWindow";

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style =  CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
	wc.lpfnWndProc = (WNDPROC)MsgHandlerSimpleOpenGLClass;
	wc.cbClsExtra = 0; wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_MENUBAR+1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = windowClassName;

	RegisterClassEx(&wc);

	HWND hWndFake = CreateWindow(windowClassName,
		windowClassName,
		WS_OVERLAPPEDWINDOW | WS_MAXIMIZE | WS_CLIPCHILDREN,
		0, 0, CW_USEDEFAULT, CW_USEDEFAULT, 
		NULL, NULL, hInstance, NULL
	);

	HDC hdc = GetDC(hWndFake);

	// First, choose false pixel format
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion   = 1;
	pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;
 
	int iPixelFormat = ChoosePixelFormat(hdc, &pfd);
	if (iPixelFormat == 0)
		return false;

	if(!SetPixelFormat(hdc, iPixelFormat, &pfd))
		return false;

	// Create the false, old style context (OpenGL 2.1 and before)

	HGLRC hrcFake = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hrcFake);

	bool bResult = true;

	GLenum status = glewInit();
	if(status != GLEW_OK)
	{
		SN_ERROR("Couldn't initialize GLEW! " << glewGetErrorString(status));
		bResult = false;
	}
	initialized = true;

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hrcFake);
	DestroyWindow(hWndFake);

	return bResult;
}

} // namespace render
} // namespace sn

