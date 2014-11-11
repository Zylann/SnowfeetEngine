#ifndef __HEADER_WINDOW_WIN32__
#define __HEADER_WINDOW_WIN32__

#include <windows.h>

namespace sn
{

class WindowImpl
{
public:

    WindowImpl(Window & win) :
        r_window(win)
    {}

    void onEvent(UINT message, WPARAM wParam, LPARAM lParam);

    static LRESULT CALLBACK windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

private:

    Window & r_window;

};

} // namespace sn

#endif // __HEADER_WINDOW_WIN32__

