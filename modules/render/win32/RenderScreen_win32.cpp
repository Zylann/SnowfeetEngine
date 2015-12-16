#include "../RenderScreen.h"
#include "GLContext_win32.h"

namespace sn
{

class RenderScreenImpl
{
public:
    RenderScreenImpl(RenderScreen & screen);
    ~RenderScreenImpl();

    void updatePixelFormat(const GLContext & context);
    bool makeCurrent(const GLContext & context);
    void swapBuffers();

private:
    RenderScreen & r_renderScreen;
    bool m_pixelFormatInitialized;
    HDC m_hdc;
    GLContextSettings m_lastExternalContextSettings;
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

RenderScreenImpl::RenderScreenImpl(RenderScreen & screen) : 
    r_renderScreen(screen), 
    m_pixelFormatInitialized(false),
    m_hdc(nullptr)
{
    HWND hwnd = static_cast<HWND>(r_renderScreen.getWindow().getHandle());
    m_hdc = GetDC(hwnd);
}

RenderScreenImpl::~RenderScreenImpl()
{
    if (m_hdc)
    {
        HWND hwnd = static_cast<HWND>(r_renderScreen.getWindow().getHandle());
        ReleaseDC(hwnd, m_hdc);
    }
}

void RenderScreenImpl::updatePixelFormat(const GLContext & context)
{
    HWND hwnd = static_cast<HWND>(r_renderScreen.getWindow().getHandle());
    GLContextSettings settings = context.getSettings();

    if (!m_pixelFormatInitialized || settings != m_lastExternalContextSettings)
    {
        GLContextImpl::setPixelFormat(hwnd, settings);
        m_lastExternalContextSettings = settings;
        m_pixelFormatInitialized = true;
    }
}

bool RenderScreenImpl::makeCurrent(const GLContext & context)
{
    HWND hwnd = static_cast<HWND>(r_renderScreen.getWindow().getHandle());
    if (hwnd)
    {
        updatePixelFormat(context);

        HGLRC hrc = context.getImpl()->getHandle();
        HDC hdc = m_hdc;

        return hrc && hdc && wglMakeCurrent(hdc, hrc);
    }
    return false;
}

void RenderScreenImpl::swapBuffers()
{
    if (m_hdc)
        SwapBuffers(m_hdc);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void RenderScreen::initImpl()
{
    SN_ASSERT(m_impl == nullptr, "Invalid state: impl is not null in initImpl()");
    m_impl = new RenderScreenImpl(*this);
}

void RenderScreen::deinitImpl()
{
    if (m_impl)
        delete m_impl;
}

bool RenderScreen::makeCurrentImpl(GLContext & context)
{
    if (r_window.getHandle())
    {
        return m_impl->makeCurrent(context);
    }
    return false;
}

void RenderScreen::swapBuffers()
{
    m_impl->swapBuffers();
}

} // namespace sn


