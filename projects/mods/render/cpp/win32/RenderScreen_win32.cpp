#include "../RenderScreen.hpp"
#include "Context_win32.hpp"

namespace sn {
namespace render {

class RenderScreenImpl
{
public:
    RenderScreenImpl(RenderScreen & screen);
    ~RenderScreenImpl();

    void updatePixelFormat(const Context & context);
    bool makeCurrent(const Context & context);
    void swapBuffers();

private:
    RenderScreen & r_renderScreen;
    bool m_pixelFormatInitialized;
    HDC m_hdc;
    ContextSettings m_lastExternalContextSettings;
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

void RenderScreenImpl::updatePixelFormat(const Context & context)
{
    HWND hwnd = static_cast<HWND>(r_renderScreen.getWindow().getHandle());
    ContextSettings settings = context.getSettings();

    if (!m_pixelFormatInitialized || settings != m_lastExternalContextSettings)
    {
        ContextImpl::setPixelFormat(hwnd, settings);
        m_lastExternalContextSettings = settings;
        m_pixelFormatInitialized = true;
    }
}

bool RenderScreenImpl::makeCurrent(const Context & context)
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

bool RenderScreen::makeCurrentImpl(Context & context)
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

//static RenderScreen * g_currentScreen = nullptr;

// Static
bool RenderScreen::setCurrent(RenderScreen * screen, Context * context)
{
    //if (g_currentScreen == screen)
    //    return true;
    //g_currentScreen = screen;
    if (screen)
        return screen->makeCurrent(context);
    else
        return !!wglMakeCurrent(nullptr, nullptr);
}

} // namespace render
} // namespace sn


