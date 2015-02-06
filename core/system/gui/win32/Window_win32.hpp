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

enum CursorType
{
    SN_CURSOR_INVISIBLE = 0,
    SN_CURSOR_DEFAULT
};

class WindowImpl
{
public:

    WindowImpl(Window & win) :
        r_window(win),
        m_cursor(NULL)
    {
    }

    void onEvent(UINT message, WPARAM wParam, LPARAM lParam);
    
    void setCursor(CursorType type);

    static LRESULT CALLBACK windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

private:

    Window & r_window;
    HCURSOR m_cursor; // Which cursor must be displayed over the window
};

} // namespace sn

#endif // __HEADER_WINDOW_WIN32__

