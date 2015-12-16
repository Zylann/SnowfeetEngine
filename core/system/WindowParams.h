#ifndef __HEADER_SN_WINDOWPARAMS__
#define __HEADER_SN_WINDOWPARAMS__

#include <core/types.h>
#include <core/math/Rect.h>
#include <string>
#include <core/sml/variant_serialize.h>

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
        WindowHandle parent_ = nullptr,
        const std::string title_ = "Window",
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
    u32 style;
};

void SN_API serialize(Variant & o, const WindowParams & params);
void SN_API unserialize(const Variant & o, WindowParams & params);

} // namespace sn

#endif // __HEADER_SN_WINDOWPARAMS__

