#include "../SystemGUI.hpp"
#include <Windows.h>

namespace sn
{

Vector2u SystemGUI::getDesktopSize() const
{
    HDC screenDC = GetDC(NULL);
    int w = GetDeviceCaps(screenDC, HORZRES);
    int h = GetDeviceCaps(screenDC, VERTRES);
    ReleaseDC(NULL, screenDC);
    return Vector2u(w,h);
}

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

