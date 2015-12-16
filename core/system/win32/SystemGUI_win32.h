#ifndef __HEADER_SN_SYSTEMGUI_WIN32__
#define __HEADER_SN_SYSTEMGUI_WIN32__

#include "../Cursor.h"
#include <Windows.h>

namespace sn
{

class SystemGUIImpl
{
public:
    SystemGUIImpl();
    //~SystemGUIImpl();

    void setMouseCursor(CursorType cursorType);

private:
    HCURSOR m_cursors[SN_CURSOR_TYPE_COUNT];

};

} // namespace sn

#endif // __HEADER_SN_SYSTEMGUI_WIN32__

