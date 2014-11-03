#include "../Window.hpp"
#include <windows.h>

namespace sn
{

//------------------------------------------------------------------------------
void Window::create(WindowParams params)
{
    if (isCreated())
    {
        destroy();
    }

    s32 dwStyle = WS_CLIPCHILDREN;

    if (params.style & SN_WS_CAPTION)
        dwStyle |= WS_CAPTION;

    if (params.style & SN_WS_MINIMIZABLE)
        dwStyle |= WS_MINIMIZEBOX;

    if (params.style & SN_WS_MAXIMIZABLE)
        dwStyle |= WS_MAXIMIZEBOX;

    // TODO handle more window flags

    HINSTANCE hAppInstance = GetModuleHandle(NULL);

    RECT winRect = {
        params.rect.minX(),
        params.rect.minY(),
        params.rect.maxX(),
        params.rect.maxY()
    };

    ::AdjustWindowRect(&winRect, dwStyle, false);

    HWND h = CreateWindowEx(
        0,
        params.winClass.c_str(),
        params.title.c_str(),
        dwStyle,
        winRect.left,
        winRect.top,
        winRect.right - winRect.left,
        winRect.bottom - winRect.top,
        NULL,
        NULL,
        hAppInstance,
        0
    );

    m_title = params.title;
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
void Window::destroy()
{
    if (m_handle)
    {
        ::DestroyWindow(reinterpret_cast<HWND>(m_handle));
        m_handle = nullptr;
    }
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




