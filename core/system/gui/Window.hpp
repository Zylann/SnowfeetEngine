/*
Window.hpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_SN_WINDOW__
#define __HEADER_SN_WINDOW__

#include <core/math/Rect.hpp>
//#include <core/system/gui/Layout.hpp>
#include <core/util/NonCopyable.hpp>

#include <string>

namespace sn
{

//------------------------------------------------------------------------------
typedef void* WindowHandle;

//------------------------------------------------------------------------------
enum WindowStyle
{
    SN_WS_BORDER = 1, // Client border
    SN_WS_CLOSABLE = 2, // Close button
    SN_WS_MAXIMIZABLE = 4, // Maximize button
    SN_WS_MINIMIZABLE = 8, // Minimize button
    SN_WS_SHOWN = 16, // Show on create
    SN_WS_FULLSCREEN = 32,
    SN_WS_CAPTION = 64, // Title bar
    SN_WS_RESIZEABLE = 128,
    SN_WS_CENTERED = 256,

    _SN_WS_DEFAULT = SN_WS_BORDER
        | SN_WS_CLOSABLE
        | SN_WS_MAXIMIZABLE
        | SN_WS_MINIMIZABLE
        | SN_WS_SHOWN
        | SN_WS_CAPTION
        | SN_WS_RESIZEABLE
        | SN_WS_CENTERED
};

//------------------------------------------------------------------------------
struct SN_API WindowParams
{
    WindowParams(
        WindowHandle parent_,
        const std::string title_,
        IntRect rect_ = IntRect(0, 0, 800, 480),
        u32 style = _SN_WS_DEFAULT
    ) :
        parent(parent_),
        rect(rect_),
        title(title_),
        style(style)
    {}

    WindowHandle parent;
    IntRect rect;
    std::string title;
    unsigned int style;
};

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


