#include "RenderScreen.hpp"

namespace sn {
namespace render {

RenderScreen::RenderScreen(Window & window):
    m_context(nullptr),
    r_window(window),
    m_impl(nullptr)
{
}

RenderScreen::~RenderScreen()
{
    deinitImpl();
    if (m_context)
        delete m_context;
}

void RenderScreen::createSharedContext(Context & mainContext, ContextSettings settings)
{
    if (m_context)
        delete m_context;
    m_context = new Context(settings, &r_window, &mainContext);
}

bool RenderScreen::makeCurrent(Context * context)
{
    if (context == nullptr)
    {
        SN_ASSERT(m_context != nullptr, "Cannot set current RenderScreen, context is null");
        context = m_context;
    }
    return makeCurrentImpl(*context);
}

} // namespace render
} // namespace sn

