#include "../../util/assert.hpp"
#include "WindowContainer.hpp"

namespace sn
{

//------------------------------------------------------------------------------
WindowContainer::WindowContainer(SystemGUI & gui, WindowParams params) :
    Window(gui, params)
{
}

//------------------------------------------------------------------------------
WindowContainer::~WindowContainer()
{
    auto children = m_children;
    for (auto it = children.begin(); it != children.end(); ++it)
    {
        delete (*it);
    }

    clearLayout();
}

//------------------------------------------------------------------------------
void WindowContainer::onAddChild(Window * child)
{
    SN_ASSERT(m_children.find(child) == m_children.end(), "Window: child added twice!");
    m_children.insert(child);
}

//------------------------------------------------------------------------------
void WindowContainer::onRemoveChild(Window * child)
{
    SN_ASSERT(m_children.find(child) != m_children.end(), "Window: child not present on remove!");
    m_children.erase(child);
}

//------------------------------------------------------------------------------
void WindowContainer::clearLayout()
{
    if (m_layout)
        delete m_layout;
    m_layout = nullptr;
}

} // namespace sn

