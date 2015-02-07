/*
Window_win32.hpp
Copyright (C) 2014-2015 Marc GILLERON
This file is part of the SnowfeetEngine project.
*/

#include "../../../util/assert.hpp"
#include "../Window.hpp"
#include "../SystemGUI.hpp"
#include "../Event.hpp"

#include "Window_win32.hpp"

namespace sn
{

//==============================================================================
// Global
//==============================================================================

const char * g_classNameA = "SnowfeetWindowA";

//------------------------------------------------------------------------------
void registerWindowClass()
{
    WNDCLASSA winClass;
    winClass.style = 0;
    winClass.lpfnWndProc = &WindowImpl::windowProc;
    winClass.cbClsExtra = 0;
    winClass.cbWndExtra = 0;
    winClass.hInstance = GetModuleHandle(NULL);
    winClass.hIcon = NULL;
    winClass.hCursor = 0;
    winClass.hbrBackground = 0;
    winClass.lpszMenuName = NULL;
    winClass.lpszClassName = g_classNameA;
    ::RegisterClassA(&winClass);
}

//==============================================================================
// WindowImpl
//==============================================================================

//------------------------------------------------------------------------------
void WindowImpl::onEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
    // Don't process any message until window is created
    if (r_window.getHandle() == NULL)
        return;

    SystemGUI & manager = r_window.getManager();

    Event e(SN_EVENT_RAW);
    e.raw.wparam = wParam;
    e.raw.lparam = lParam;

    e.windowID = manager.getWindowID(&r_window);

    switch (message)
    {
    case WM_DESTROY:
        // Show cursor if it was hidden
        setCursor(SN_CURSOR_DEFAULT);
        break;

    case WM_CLOSE:
        e.type = SN_EVENT_WINDOW_CLOSED;
        manager.pushEvent(e);
        break;

    case WM_SIZE:
    {
        e.type = SN_EVENT_WINDOW_RESIZED;
        IntRect r = r_window.getClientRect();
        e.window.x = r.x;
        e.window.y = r.y;
        e.window.width = r.width;
        e.window.height = r.height;
        manager.pushEvent(e);
    }
        break;

    case WM_SETFOCUS:
    {
        e.type = SN_EVENT_WINDOW_GAINED_FOCUS;
        manager.pushEvent(e);
    }
        break;

    case WM_KILLFOCUS:
    {
        e.type = SN_EVENT_WINDOW_LOST_FOCUS;
        manager.pushEvent(e);
        break;
    }

    case WM_SETCURSOR:
        // The mouse has moved, if the cursor is in our window we must refresh the cursor
        if (LOWORD(lParam) == HTCLIENT)
            ::SetCursor(m_cursor);
        break;

    default:
        manager.pushEvent(e);
        break;
    }
}

//------------------------------------------------------------------------------
void WindowImpl::setCursor(CursorType type)
{
    switch (type)
    {
    case SN_CURSOR_INVISIBLE:
        m_cursor = NULL;
        break;

    default: // SN_CURSOR_DEFAULT:
        m_cursor = LoadCursor(NULL, IDC_ARROW);
        break;
    }

    ::SetCursor(m_cursor);
}

//------------------------------------------------------------------------------
// Static
LRESULT CALLBACK WindowImpl::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
    // Associate handle and Window instance when the creation message is received
    if (message == WM_CREATE)
    {
        // Get WindowImplWin32 instance (it was passed as the last argument of CreateWindow)
        LONG_PTR window = (LONG_PTR)reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams;

        // Set as the "userdata" parameter of the window
        SetWindowLongPtr(handle, GWLP_USERDATA, window);
    }

    // Get the WindowImpl instance corresponding to the window handle
    WindowImpl * impl = handle ? reinterpret_cast<WindowImpl*>(GetWindowLongPtr(handle, GWLP_USERDATA)) : NULL;

    // Forward the event to the appropriate function
    if (impl)
    {
        impl->onEvent(message, wParam, lParam);

        //if (window->m_callback)
        //    return CallWindowProc(reinterpret_cast<WNDPROC>(window->m_callback), handle, message, wParam, lParam);
    }

    // We don't forward the WM_CLOSE message to prevent the OS from automatically destroying the window
    if (message == WM_CLOSE)
        return 0;

    // Don't forward the menu system command, so that pressing ALT or F10 doesn't steal the focus
    if ((message == WM_SYSCOMMAND) && (wParam == SC_KEYMENU))
        return 0;

    //static const bool hasUnicode = hasUnicodeSupport();
    //return hasUnicode ? DefWindowProcW(handle, message, wParam, lParam) :
    return DefWindowProcA(handle, message, wParam, lParam);
}

//==============================================================================
// Window
//==============================================================================

//------------------------------------------------------------------------------
void Window::onCreate(WindowParams params, bool isFirst)
{
    //if (isCreated())
    //{
    //    onDestroy();
    //}

    SN_ASSERT(m_handle == 0, "Same window created twice");

    if (isFirst)
    {
        registerWindowClass();
    }

    s32 dwStyle = WS_CLIPCHILDREN;

    if (params.style & SN_WS_RESIZEABLE)
        dwStyle |= WS_SIZEBOX;

    if (params.style & SN_WS_CAPTION)
        dwStyle |= WS_CAPTION;

    if (params.style & SN_WS_MINIMIZABLE)
        dwStyle |= WS_MINIMIZEBOX;

    if (params.style & SN_WS_MAXIMIZABLE)
        dwStyle |= WS_MAXIMIZEBOX;

    if (params.style & SN_WS_CLOSABLE)
        dwStyle |= WS_SYSMENU;

    if (params.style & SN_WS_SHOWN)
        dwStyle |= WS_VISIBLE;

    HINSTANCE hAppInstance = GetModuleHandle(NULL);

    RECT winRect = {
        params.rect.minX(),
        params.rect.minY(),
        params.rect.maxX(),
        params.rect.maxY()
    };

    ::AdjustWindowRect(&winRect, dwStyle, false);

    m_impl = new WindowImpl(*this);

    HWND h = CreateWindowEx(
        0,
        g_classNameA,
        params.title.c_str(),
        dwStyle,
        winRect.left,
        winRect.top,
        winRect.right - winRect.left,
        winRect.bottom - winRect.top,
        reinterpret_cast<HWND>(params.parent),
        NULL,
        hAppInstance,
        m_impl // userdata
    );

    m_title = params.title;
    m_handle = h;
}

//------------------------------------------------------------------------------
void Window::onDestroy()
{
    if (m_handle)
    {
        ::DestroyWindow(reinterpret_cast<HWND>(m_handle));
        m_handle = nullptr;
    }

    delete m_impl;
    m_impl = 0;
}

//------------------------------------------------------------------------------
void Window::setVisible(bool visible)
{
    HWND h = reinterpret_cast<HWND>(m_handle);
    if (visible)
        ::ShowWindow(h, SW_SHOWNOACTIVATE);
    else
        ::ShowWindow(h, SW_HIDE);
}

//------------------------------------------------------------------------------
IntRect Window::getRect() const
{
    RECT rc;
    ::GetWindowRect(reinterpret_cast<HWND>(m_handle), &rc);
    return IntRect::fromMinMax(rc.left, rc.top, rc.right, rc.bottom);
}

//------------------------------------------------------------------------------
void Window::setRect(IntRect rect)
{
    ::SetWindowPos(
        reinterpret_cast<HWND>(m_handle),
        0,
        rect.minX(),
        rect.minY(),
        rect.width,
        rect.height,
        SWP_NOZORDER | SWP_NOACTIVATE
    );
}

//------------------------------------------------------------------------------
IntRect Window::getClientRect() const
{
    RECT rc = { 0, 0, 0, 0 };
    if (m_handle)
    {
        ::GetClientRect(reinterpret_cast<HWND>(m_handle), &rc);
    }
    return IntRect::fromMinMax(rc.left, rc.top, rc.right, rc.bottom);
}

//------------------------------------------------------------------------------
void Window::setClientRect(IntRect rect)
{
    if (m_handle)
    {
        RECT rc = {
            rect.minX(),
            rect.minY(),
            rect.maxX(),
            rect.maxY()
        };

        LONG windowStyle = GetWindowLongPtr(reinterpret_cast<HWND>(m_handle), GWL_STYLE);
        ::AdjustWindowRect(&rc, windowStyle, false);

        setRect(IntRect::fromMinMax(rc.left, rc.top, rc.right, rc.bottom));
    }
}

//------------------------------------------------------------------------------
void Window::invalidateAll()
{
    ::InvalidateRect(reinterpret_cast<HWND>(m_handle), NULL, FALSE);
}

//------------------------------------------------------------------------------
void Window::setTitle(const std::string & title)
{
    ::SetWindowText(reinterpret_cast<HWND>(m_handle), title.c_str());
    m_title = title;
}

} // namespace sn




