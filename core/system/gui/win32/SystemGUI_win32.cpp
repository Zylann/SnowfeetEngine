#include "../SystemGUI.hpp"
#include <Windows.h>

namespace sn
{

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

