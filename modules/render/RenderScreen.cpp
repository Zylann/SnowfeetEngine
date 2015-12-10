#include "RenderScreen.h"

namespace sn
{

RenderScreen::RenderScreen(Window & window):
    m_context(nullptr),
    r_window(window),
    m_impl(nullptr)
{
    initImpl();
}

RenderScreen::~RenderScreen()
{
    deinitImpl();
    if (m_context)
        delete m_context;
}

void RenderScreen::createSharedContext(GLContext & mainContext, GLContextSettings settings)
{
    if (m_context)
        delete m_context;
    m_context = new GLContext(settings, &r_window, &mainContext);
}

bool RenderScreen::makeCurrent(GLContext & context)
{
    return makeCurrentImpl(context);
}

const Vector2u RenderScreen::getSize()
{
    return r_window.getClientSize();
}

} // namespace sn

