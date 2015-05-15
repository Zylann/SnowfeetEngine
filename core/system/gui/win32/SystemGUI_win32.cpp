#include "../SystemGUI.hpp"
#include "SystemGUI_win32.hpp"

namespace sn
{

SystemGUIImpl::SystemGUIImpl()
{
    memset(m_cursors, 0, sizeof(m_cursors));
}

//SystemGUIImpl::~SystemGUIImpl()
//{
//
//}

void SystemGUIImpl::setMouseCursor(CursorType cursorType)
{
    HCURSOR hCursor = m_cursors[cursorType];

    if (hCursor == nullptr)
    {
        LPCTSTR cursorID = nullptr;
        switch (cursorType)
        {
        case SN_CURSOR_INVISIBLE:                     cursorID = nullptr; break;
        case SN_CURSOR_DEFAULT:                       cursorID = IDC_ARROW; break;
        case SN_CURSOR_RESIZE_HORIZONTAL:             cursorID = IDC_SIZEWE; break;
        case SN_CURSOR_RESIZE_VERTICAL:               cursorID = IDC_SIZENS; break;
        case SN_CURSOR_RESIZE_BOTTOM_LEFT_TOP_RIGHT:  cursorID = IDC_SIZENESW; break;
        case SN_CURSOR_RESIZE_TOP_LEFT_BOTTOM_RIGHT:  cursorID = IDC_SIZENWSE; break;
        case SN_CURSOR_RESIZE_MULTI:                  cursorID = IDC_SIZEALL; break;
        case SN_CURSOR_TEXT_EDIT:                     cursorID = IDC_IBEAM; break;
        default:                                      cursorID = IDC_ARROW; break;
        }

        if (cursorID)
        {
            hCursor = ::LoadCursor(NULL, cursorID);
            m_cursors[cursorType] = hCursor;
        }
    }

    // Note: the cursor will only change if it is different from the previous one.
    ::SetCursor(hCursor);
}

void SystemGUI::initImpl()
{
    SN_ASSERT(m_impl == nullptr, "Impl already initialized");
    m_impl = new SystemGUIImpl();
}

void SystemGUI::deinitImpl()
{
    SN_ASSERT(m_impl != nullptr, "Impl already destroyed or not created");
    delete m_impl;
    m_impl = nullptr;
}

void SystemGUI::setMouseCursor(CursorType cursorType)
{
    m_impl->setMouseCursor(cursorType);
}

// Static
Vector2u SystemGUI::getDesktopSize()
{
    HDC screenDC = GetDC(NULL);
    int w = GetDeviceCaps(screenDC, HORZRES);
    int h = GetDeviceCaps(screenDC, VERTRES);
    ReleaseDC(NULL, screenDC);
    return Vector2u(w,h);
}

// Static
void SystemGUI::processEvents()
{
    MSG message;
    // hwnd=NULL means all windows in the current thread
    while (PeekMessageW(&message, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
}

} // namespace sn

