#include <core/util/Log.hpp>
#include <core/util/assert.hpp>
#include "Context_win32.hpp"
#include <GL/glew.h>
#include <GL/wglew.h>

namespace sn {
namespace render {

//==============================================================================
// ContextImpl
//==============================================================================


// https://www.opengl.org/discussion_boards/showthread.php/173031-Easy-OpenGL-context-creation

ContextImpl::ContextImpl(Context & context, const Window & win) :
    r_context(context),
    m_hrc(nullptr),
    m_dc(nullptr)
{
    HWND hwnd = (HWND)(win.getHandle());
    HGLRC hrcBase = ContextImpl::glCreateMinimalContext(hwnd);

    if (hrcBase)
    {
        const ContextSettings & settings = context.getSettings();

        HGLRC hrc = ContextImpl::glCreateContext(hwnd, hrcBase, settings.majorVersion, settings.minorVersion);
        m_hrc = hrc; // If NULL, isInitialized() will then return false
        if (m_hrc)
            m_dc = GetDC(hwnd);
    }
    else
    {
        SN_ERROR("Couldn't initialize OpenGL context");
    }
}

ContextImpl::~ContextImpl()
{
    // Destroy the OpenGL context
    if (m_hrc)
    {
        if (wglGetCurrentContext() == m_hrc)
            wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(m_hrc);
    }

    // Destroy the device context
    if (m_dc)
    {
        // TODO Allow to create a context without a window
        HWND hwnd = (HWND)(r_context.getWindow()->getHandle());
        ReleaseDC(hwnd, m_dc);
    }
}

bool ContextImpl::makeCurrent()
{
    return m_hrc && m_dc && wglMakeCurrent(m_dc, m_hrc);
}

// Static
HGLRC ContextImpl::glCreateMinimalContext(HWND hwnd)
{
    HDC hdc = GetDC(hwnd);
    PIXELFORMATDESCRIPTOR pfd;

    // Create pixel format descriptor
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 32;
    pfd.iLayerType = PFD_MAIN_PLANE;

    // Choose pixel format and attempt to set it
    int pixelFormat = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, pixelFormat, &pfd);

    // Create the OpenGL context
    HGLRC hrc = wglCreateContext(hdc);
    if (hrc != nullptr)
    {
        wglMakeCurrent(hdc, hrc);

        // Initialize GLEW
        GLenum err = glewInit();
        if (err != GLEW_OK)
        {
            wglDeleteContext(hrc);
            hrc = nullptr;
            SN_ERROR("Failed to initialize GLEW");
        }
    }
    else
    {
        SN_ERROR("Failed to create minimal OpenGL context");
    }

    ReleaseDC(hwnd, hdc);

    return hrc;
}

// Static
HGLRC ContextImpl::glCreateContext(HWND hwnd, HGLRC sharedContext, int majorVersion, int minorVersion, int multiSampleMode)
{
    SN_ASSERT(sharedContext != nullptr, "A minimal context is needed to create an OpenGL context");

    HDC hdc = GetDC(hwnd);
    HGLRC hrc = nullptr;

    // Create format descriptor
    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 32;
    pfd.iLayerType = PFD_MAIN_PLANE;

    // Use forward-compatible context for version 3.3+
    int contextFlags = 0;
    if ((majorVersion >= 3 && minorVersion >= 3) || majorVersion > 3)
    {
        contextFlags = WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB;
    }

    // Choose pixel format and attempt to set it
    int pixelFormat;
    float fAttributes[] = { 0, 0 };
    unsigned int countFormats = 0;
    if (multiSampleMode > 1)
    {
        int iAttributes[] = {
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
            WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
            WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
            WGL_COLOR_BITS_ARB, 24,
            WGL_ALPHA_BITS_ARB, 8,
            WGL_DEPTH_BITS_ARB, 24,
            WGL_STENCIL_BITS_ARB, GL_TRUE,
            WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
            WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
            WGL_SAMPLES_ARB, multiSampleMode,
            0, 0
        };
        wglChoosePixelFormatARB(hdc, iAttributes, fAttributes, 1, &pixelFormat, &countFormats);
    }
    else
    {
        int iAttributes[] = {
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
            WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
            WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
            WGL_COLOR_BITS_ARB, 24,
            WGL_ALPHA_BITS_ARB, 8,
            WGL_DEPTH_BITS_ARB, 24,
            WGL_STENCIL_BITS_ARB, 0,
            WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
            WGL_SAMPLE_BUFFERS_ARB, GL_FALSE,
            0, 0
        };
        wglChoosePixelFormatARB(hdc, iAttributes, fAttributes, 1, &pixelFormat, &countFormats);
    }
    if (countFormats != 0)
    {
        if (SetPixelFormat(hdc, pixelFormat, &pfd))
        {
            // Attributes for the real OpenGL context
            int attribs[] = {
                WGL_CONTEXT_MAJOR_VERSION_ARB, majorVersion,
                WGL_CONTEXT_MINOR_VERSION_ARB, minorVersion,
                WGL_CONTEXT_FLAGS_ARB, contextFlags,
                0, 0
            };

            // Create context
            hrc = wglCreateContextAttribsARB(hdc, sharedContext, attribs);
            if (hrc == nullptr)
            {
                SN_ERROR("Failed to create OpenGL context");
            }
        }
        else
        {
            SN_ERROR("SetPixelFormat failed");
        }
    }
    else
    {
        SN_ERROR("wglChoosePixelFormatARB failed");
    }

    ReleaseDC(hwnd, hdc);

    return hrc;
}

void ContextImpl::swapBuffers()
{
    if (m_dc)
        SwapBuffers(m_dc);
}

//==============================================================================
// Context
//==============================================================================

void Context::initImpl(const Window & owner)
{
    m_impl = new ContextImpl(*this, owner);
}

void Context::deinitImpl()
{
    if (m_impl)
        delete m_impl;
}

bool Context::makeCurrent()
{
    return m_impl->makeCurrent();
}

void Context::swapBuffers()
{
    m_impl->swapBuffers();
}

} // namespace render
} // namespace sn

