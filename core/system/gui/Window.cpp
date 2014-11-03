#include "Window.hpp"
#include "../../util/Log.hpp"
#include <map>

namespace sn
{

//------------------------------------------------------------------------------
std::map<WindowHandle, Window*> g_handleToWindow;

//------------------------------------------------------------------------------
// Static
Window * Window::getByHandle(WindowHandle handle)
{
    auto it = g_handleToWindow.find(handle);
    if (it != g_handleToWindow.end())
        return it->second;
    else
        return nullptr;
}

//------------------------------------------------------------------------------
Window::Window(WindowParams params):
    m_handle(nullptr),
    m_parent(nullptr)
{
    create(params);

    g_handleToWindow[m_handle] = this;

    if (params.parent)
    {
        m_parent = Window::getByHandle(params.parent);
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

    g_handleToWindow.erase(m_handle);

    auto children = m_children;
    for (auto it = children.begin(); it != children.end(); ++it)
    {
        delete (*it);
    }

    clearLayout();

    destroy();
}

//------------------------------------------------------------------------------
void Window::onAddChild(Window * child)
{
    SN_ASSERT(m_children.find(child) == m_children.end(), "Child added twice!");
    m_children.insert(child);
}

//------------------------------------------------------------------------------
void Window::onRemoveChild(Window * child)
{
    SN_ASSERT(m_children.find(child) != m_children.end(), "Child not present on remove!");
    m_children.erase(child);
}

//------------------------------------------------------------------------------
void Window::clearLayout()
{
    if (m_layout)
        delete m_layout;
    m_layout = nullptr;
}


} // namespace sn


