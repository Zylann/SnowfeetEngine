#include <core/util/Log.h>
#include <core/util/assert.h>
#include <core/system/Lock.h>

#include "../GLExtensions.h"

//#include "../gl_check.h"
#include "Context_win32.h"

#include <GL/glew.h>
#include <GL/wglew.h>

#include <tchar.h>
#include <core/system/win32/helpers_win32.h>

namespace sn {
namespace render {


//==============================================================================
// ContextImpl
//==============================================================================


// https://www.opengl.org/discussion_boards/showthread.php/173031-Easy-OpenGL-context-creation

//------------------------------------------------------------------------------
ContextImpl::ContextImpl(Context & context, ContextSettings & settings, ContextImpl * sharedContext) :
	r_context(context),
    m_hrc(nullptr),
    m_dc(nullptr),
    m_ownHwnd(nullptr)
{
    // Get the HWND from either the passed window or an internal one
    HWND hwnd = nullptr;
	const Window * win = r_context.getWindow();
    if (win)
    {
        // A window was given, go with it
    	hwnd = (HWND)win->getHandle();
    }
    else
    {
        // No window was given...
        // On Windows, we still have to create a dummy window to get a valid DC
        m_ownHwnd = ::CreateWindow("STATIC", "", WS_POPUP | WS_DISABLED, 0, 0, 1, 1, NULL, NULL, GetModuleHandle(NULL), NULL);
        ShowWindow(m_ownHwnd, SW_HIDE);
        hwnd = m_ownHwnd;
    }

	m_dc = GetDC(hwnd);

	if (m_dc)
	{
        ContextSettings initialSettings = settings;
		createContext(sharedContext ? sharedContext->m_hrc : nullptr, settings, hwnd);
		if (settings != initialSettings)
		{
			SN_WARNING("ContextSettings have been changed for compatibility.");
			SN_LOG("Requested: " << r_context.getSettings().toString());
			SN_LOG("Changed to: " << settings.toString());
		}
	}
}

//------------------------------------------------------------------------------
ContextImpl::~ContextImpl()
{
    // Destroy the OpenGL context
    if (m_hrc)
    {
        if (wglGetCurrentContext() == m_hrc)
            wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(m_hrc);
    }

    // Release the device context
    if (m_dc)
    {
        // TODO Allow to create a context without a window
        HWND hwnd = m_ownHwnd ? m_ownHwnd : (HWND)(r_context.getWindow()->getHandle());
        ReleaseDC(hwnd, m_dc);
    }

    // Destroy the owned window if any
    if (m_ownHwnd)
    {
        ::DestroyWindow(m_ownHwnd);
    }
}

//------------------------------------------------------------------------------
bool ContextImpl::makeCurrent(bool isCurrent)
{
    if (isCurrent)
        return m_hrc && m_dc && wglMakeCurrent(m_dc, m_hrc);
    else
        return !!wglMakeCurrent(nullptr, nullptr);
}

//------------------------------------------------------------------------------
// Static
bool ContextImpl::setPixelFormat(HWND hwnd, ContextSettings & settings)
{
    ensureGLExtensions();

    HDC hdc = GetDC(hwnd);
    //HGLRC hrc = nullptr;

	u32 bitsPerPixel = 32;

    // Let's find a suitable pixel format -- first try with antialiasing
    int bestFormat = 0;
    if (settings.multiSampleLevel > 0)
    {
        // Get the wglChoosePixelFormatARB function (it is an extension)
        //PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = reinterpret_cast<PFNWGLCHOOSEPIXELFORMATARBPROC>(wglGetProcAddress("wglChoosePixelFormatARB"));
		if (wglChoosePixelFormatARB)
		{
			// Define the basic attributes we want for our window
			int intAttributes[] =
			{
				WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
				WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
				WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
				WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
				WGL_SAMPLE_BUFFERS_ARB, (settings.multiSampleLevel ? GL_TRUE : GL_FALSE),
				WGL_SAMPLES_ARB, settings.multiSampleLevel,
				0, 0
			};

			// Let's check how many formats are supporting our requirements
			int   formats[128];
			UINT  nbFormats;
			float floatAttributes[] = { 0, 0 };
			bool  isValid = wglChoosePixelFormatARB(hdc, intAttributes, floatAttributes, sizeof(formats) / sizeof(*formats), formats, &nbFormats) != 0;
			while ((!isValid || (nbFormats == 0)) && settings.multiSampleLevel > 0)
			{
				// Decrease the antialiasing level until we find a valid one
				--settings.multiSampleLevel;
				intAttributes[11] = settings.multiSampleLevel;
				isValid = wglChoosePixelFormatARB(hdc, intAttributes, floatAttributes, sizeof(formats) / sizeof(*formats), formats, &nbFormats) != 0;
			}

			// Get the best format among the returned ones
			if (isValid && (nbFormats > 0))
			{
				int bestScore = 0xFFFF;
				for (UINT i = 0; i < nbFormats; ++i)
				{
					// Get the current format's attributes
					PIXELFORMATDESCRIPTOR attributes;
					attributes.nSize = sizeof(attributes);
					attributes.nVersion = 1;
					DescribePixelFormat(hdc, formats[i], sizeof(attributes), &attributes);

					// Evaluate the current configuration
					int color = attributes.cRedBits + attributes.cGreenBits + attributes.cBlueBits + attributes.cAlphaBits;
					int score = settings.evaluate(bitsPerPixel, color, attributes.cDepthBits, attributes.cStencilBits, settings.multiSampleLevel);

					// Keep it if it's better than the current best
					if (score < bestScore)
					{
						bestScore = score;
						bestFormat = formats[i];
					}
				}
			}
		}
        else
        {
            // wglChoosePixelFormatARB not supported ; disabling antialiasing
            SN_WARNING("Antialiasing is not supported ; it will be disabled");
            settings.multiSampleLevel = 0;
        }
    }

    // Find a pixel format with no antialiasing, if not needed or not supported
    if (bestFormat == 0)
    {
        // Setup a pixel format descriptor from the rendering settings
        PIXELFORMATDESCRIPTOR descriptor;
        ZeroMemory(&descriptor, sizeof(descriptor));
        descriptor.nSize        = sizeof(descriptor);
        descriptor.nVersion     = 1;
        descriptor.iLayerType   = PFD_MAIN_PLANE;
        descriptor.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        descriptor.iPixelType   = PFD_TYPE_RGBA;
        descriptor.cColorBits   = static_cast<BYTE>(bitsPerPixel);
        descriptor.cDepthBits   = static_cast<BYTE>(settings.depthBits);
        descriptor.cStencilBits = static_cast<BYTE>(settings.stencilBits);
        descriptor.cAlphaBits   = bitsPerPixel == 32 ? 8 : 0;

        // Get the pixel format that best matches our requirements
        bestFormat = ChoosePixelFormat(hdc, &descriptor);
        if (bestFormat == 0)
        {
            SN_ERROR("Failed to find a suitable pixel format for device context -- cannot create OpenGL context");
			ReleaseDC(hwnd, hdc);
			return false;
        }
    }

    // Extract the depth and stencil bits from the chosen format
    PIXELFORMATDESCRIPTOR actualFormat;
    actualFormat.nSize    = sizeof(actualFormat);
    actualFormat.nVersion = 1;
    DescribePixelFormat(hdc, bestFormat, sizeof(actualFormat), &actualFormat);
    settings.depthBits   = actualFormat.cDepthBits;
    settings.stencilBits = actualFormat.cStencilBits;

    // Set the chosen pixel format
    if (!SetPixelFormat(hdc, bestFormat, &actualFormat))
    {
        SN_ERROR("Failed to set pixel format for device context -- cannot create OpenGL context");
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
void ContextImpl::createContext(HGLRC sharedContext, ContextSettings & settings, HWND hwnd)
{
    if (!setPixelFormat(hwnd, settings))
        return;

    // Get the context to share display lists with
    //HGLRC sharedContext = shared ? shared->m_context : NULL;

    // Create the OpenGL context -- first try context versions >= 3.0 if it is requested (they require special code)
    while (!m_hrc && (settings.majorVersion >= 3))
    {
        //PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(wglGetProcAddress("wglCreateContextAttribsARB"));
        if (wglCreateContextAttribsARB)
        {
            int attributes[] =
            {
                WGL_CONTEXT_MAJOR_VERSION_ARB, static_cast<int>(settings.majorVersion),
                WGL_CONTEXT_MINOR_VERSION_ARB, static_cast<int>(settings.minorVersion),
                WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
                0, 0
            };
            m_hrc = wglCreateContextAttribsARB(m_dc, sharedContext, attributes);
        }

        // If we couldn't create the context, lower the version number and try again -- stop at 3.0
        // Invalid version numbers will be generated by this algorithm (like 3.9), but we really don't care
        if (!m_hrc)
        {
            if (settings.minorVersion > 0)
            {
                // If the minor version is not 0, we decrease it and try again
                settings.minorVersion--;
            }
            else
            {
                // If the minor version is 0, we decrease the major version
                settings.majorVersion--;
                settings.minorVersion = 9;
            }
        }
    }

    // If the OpenGL >= 3.0 context failed or if we don't want one, create a regular OpenGL 1.x/2.x context
    if (!m_hrc)
    {
        // set the context version to 2.0 (arbitrary)
        settings.majorVersion = 2;
        settings.minorVersion = 0;

        m_hrc = wglCreateContext(m_dc);
        if (!m_hrc)
        {
            SN_ERROR("Failed to create an OpenGL context for this window");
            return;
        }

        // Share this context with others
        if (sharedContext)
        {
            // wglShareLists doesn't seem to be thread-safe
            static Mutex mutex;
            Lock lock(mutex);

            if (!wglShareLists(sharedContext, m_hrc))
                SN_ERROR("Failed to share the OpenGL context");
        }
    }
}

//------------------------------------------------------------------------------
//void ContextImpl::swapBuffers()
//{
//    if (m_dc)
//        SwapBuffers(m_dc);
//}

//==============================================================================
// Context
//==============================================================================

//------------------------------------------------------------------------------
void Context::initImpl(Context * sharedContext)
{
    m_impl = new ContextImpl(*this, m_settings, sharedContext ? sharedContext->m_impl : nullptr);
}

//------------------------------------------------------------------------------
void Context::deinitImpl()
{
    if (m_impl)
        delete m_impl;
}

//------------------------------------------------------------------------------
bool Context::makeCurrent(bool isCurrent)
{
    return m_impl->makeCurrent(isCurrent);
}

//------------------------------------------------------------------------------
//void Context::swapBuffers()
//{
//    m_impl->swapBuffers();
//}

} // namespace render
} // namespace sn

