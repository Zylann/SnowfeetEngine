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
SystemGUI::SystemGUI()
{
    onCreate();
}

//------------------------------------------------------------------------------
SystemGUI::~SystemGUI()
{
    while (!m_windows.empty())
    {
        delete *(m_windows.begin());
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
    m_windows.insert(&win);
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
        Window * win = *it;
        if (win->getHandle() == h)
            return win;
    }
    return nullptr;
}

} // namespace sn

