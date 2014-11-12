#include "SystemGUI.hpp"
#include "../../util/Log.hpp"

namespace sn
{

//------------------------------------------------------------------------------
SystemGUI & SystemGUI::get()
{
    static SystemGUI instance;
    return instance;
}

//------------------------------------------------------------------------------
SystemGUI::SystemGUI():
    m_nextWindowID(0)
{
    onCreate();
}

//------------------------------------------------------------------------------
SystemGUI::~SystemGUI()
{
    while (!m_windows.empty())
    {
        delete m_windows.begin()->first;
    }

    onDestroy();
}

//------------------------------------------------------------------------------
void SystemGUI::onCreate()
{

}

//------------------------------------------------------------------------------
void SystemGUI::onDestroy()
{

}

//------------------------------------------------------------------------------
void SystemGUI::refWindow(Window & win)
{
    SN_ASSERT(m_windows.find(&win) == m_windows.end(), "SystemGUI: window referenced twice");
    m_windows.insert(std::make_pair(&win, makeWindowID()));
}

//------------------------------------------------------------------------------
void SystemGUI::unrefWindow(Window & win)
{
    SN_ASSERT(m_windows.find(&win) != m_windows.end(), "SystemGUI: window not referenced");
    m_windows.erase(&win);
}

//------------------------------------------------------------------------------
Window * SystemGUI::getWindowByHandle(WindowHandle h)
{
    for (auto it = m_windows.begin(); it != m_windows.end(); ++it)
    {
        Window * win = it->first;
        if (win->getHandle() == h)
            return win;
    }
    return nullptr;
}

//------------------------------------------------------------------------------
Window * SystemGUI::getWindowByID(u32 id)
{
    for (auto it = m_windows.begin(); it != m_windows.end(); ++it)
    {
        if (it->second == id)
            return it->first;
    }
    return nullptr;
}

//------------------------------------------------------------------------------
bool SystemGUI::getWindowID(Window * win, u32 * id)
{
    auto it = m_windows.find(win);
    if (it == m_windows.end())
        return false;
    else if (id)
        *id = it->second;
    return true;
}

//------------------------------------------------------------------------------
u32 SystemGUI::makeWindowID()
{
    return m_nextWindowID++;
}

} // namespace sn

