/*
SystemGUI.hpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SYSTEM_GUI__
#define __HEADER_SYSTEM_GUI__

#include <core/system/gui/Window.hpp>
#include <core/util/NonCopyable.hpp>
#include <core/system/gui/Event.hpp>

#include <map>
#include <queue>

namespace sn
{

/// \brief Singleton manager of all system windows and events
class SN_API SystemGUI : public NonCopyable
{
public:

    // Get singleton
    static SystemGUI & get();

    SystemGUI();
    ~SystemGUI();

    Window * createWindow(WindowParams params = WindowParams(nullptr, "Window"));

    Window * getWindowByHandle(WindowHandle h);
    Window * getWindowByID(u32 id);

    /// \brief Gets the ID of a window.
    /// Asserts for invalid state if the given window isn't referenced.
    /// \param win: window
    /// \return ID
    u32 getWindowID(Window * win);

    inline u32 getWindowCount() const { return m_windows.size(); }

    // Platform-specific
    /// \brief Polls messages from the system's pump and dispatches them to all windows in the current thread.
    void processEvents();

    //--------------------------------
    // Internal
    //--------------------------------

    void refWindow(Window & win);
    void unrefWindow(Window & win);

    void pushEvent(Event e);
    bool popEvent(Event & e);

private:

    void onCreate();
    void onDestroy();

    u32 makeWindowID();

    std::map<Window*,u32> m_windows;
    std::map<u32, Window*> m_idToWindow;
    u32 m_nextWindowID;

    std::queue<Event> m_events;
};

} // namespace sn

#endif // __HEADER_SYSTEM_GUI__

