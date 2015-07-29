/*
Window.h
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_WINDOW__
#define __HEADER_SN_WINDOW__

#include <core/util/NonCopyable.h>
#include <core/math/Vector2.h>
#include <core/system/WindowParams.h>
#include <core/system/Cursor.h>

#include <string>

namespace sn
{

//------------------------------------------------------------------------------
class SystemGUI;
class WindowImpl;
class WindowContainer;

//------------------------------------------------------------------------------
/// \brief A rectangular area on the screen.
/// It's the frontier between the system's GUI and what the engine will show by itself.
class SN_API Window : public NonCopyable
{
public:

    Window(SystemGUI & manager, WindowParams params = WindowParams(nullptr, "Window"));
    virtual ~Window();

    bool isOpen() const;

    inline void show() { setVisible(true); }
    void setVisible(bool visible);
    
    inline WindowHandle getHandle() const { return m_handle; };

    void setTitle(const std::string & title);
    inline const std::string & getTitle() const { return m_title; }

    void setRect(IntRect rect);
    IntRect getRect() const;

    void setClientRect(IntRect rect);
    IntRect getClientRect() const;

    Vector2u getClientSize() const;

    /// \brief Puts the window fullscreen on a specific monitor.
    /// \param fullscreen:                wether the window should be fullscreen or not
    /// \param monitorID (optional):      ID of the monitor the window should cover.
    ///                                   If -1, the choosen monitor will be the one this window currently covers the most.
    ///                                   This parameter is only relevant if fullscreen is true.
    void setFullscreen(bool fullscreen, u32 monitorID = -1);

    /// \brief Applies post-creation parameters to the window.
    /// \note Not all parameters might be applied depending on the case.
    void applyParams(WindowParams params);

    void setMouseCursor(CursorType type);

    //virtual bool isContainer() const { return false; }

    inline SystemGUI & getManager() const { return r_manager; }

private:
    
    void onCreate(WindowParams params, bool isFirst);
    void onDestroy();

    void invalidateAll();

private:

    SystemGUI & r_manager;

    WindowHandle m_handle;
    std::string m_title;

    //WindowContainer * m_parent;

    WindowImpl * m_impl;
};

} // namespace sn

#endif // __HEADER_SN_WINDOW__


