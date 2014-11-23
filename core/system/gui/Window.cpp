#include "../../util/assert.hpp"
#include "../../util/typecheck.hpp"
#include "Window.hpp"
#include "WindowContainer.hpp"
#include "SystemGUI.hpp"
#include <map>

namespace sn
{

//------------------------------------------------------------------------------
Window::Window(SystemGUI & manager, WindowParams params):
    r_manager(manager),
    m_handle(nullptr),
    m_parent(nullptr),
    m_impl(nullptr)
{
    bool isFirst = r_manager.getWindowCount() == 0;

    r_manager.refWindow(*this);

    onCreate(params, isFirst);

    if (params.parent)
    {
        Window * pwin = r_manager.getWindowByHandle(params.parent);
        SN_ASSERT(pwin != nullptr && pwin->isContainer(), "Window: invalid parent (" << (u32)pwin << ")");
        m_parent = checked_cast<WindowContainer*>(pwin);
    }

    if (m_parent)
    {
        m_parent->onAddChild(this);
    }
}

//------------------------------------------------------------------------------
Window::~Window()
{
    if (m_parent)
    {
        m_parent->onRemoveChild(this);
    }

    onDestroy();

    r_manager.unrefWindow(*this);
}


} // namespace sn


