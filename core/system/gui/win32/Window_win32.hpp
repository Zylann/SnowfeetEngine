/*
Window_win32.hpp
Copyright (C) 2012-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#ifndef __HEADER_WINDOW_WIN32__
#define __HEADER_WINDOW_WIN32__

#include <windows.h>

namespace sn
{

/// \cond INTERNAL

class WindowImpl
{
public:

    WindowImpl(Window & win);

    void onEvent(UINT message, WPARAM wParam, LPARAM lParam);
    
    void setCursor(CursorType type);

    static LRESULT CALLBACK windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
    static KeyCode platformKeyCodeToGeneric(WPARAM key, LPARAM flags);

private:
    Window & r_window;
    CursorType m_lastCursor;
};

/// \endcond

} // namespace sn

#endif // __HEADER_WINDOW_WIN32__

