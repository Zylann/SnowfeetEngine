#ifndef __HEADER_SN_CURSOR__
#define __HEADER_SN_CURSOR__

namespace sn
{

enum CursorType
{
    SN_CURSOR_INVISIBLE = 0,
    
    SN_CURSOR_DEFAULT,

    SN_CURSOR_RESIZE_VERTICAL,
    SN_CURSOR_RESIZE_HORIZONTAL,
    SN_CURSOR_RESIZE_BOTTOM_LEFT_TOP_RIGHT,
    SN_CURSOR_RESIZE_TOP_LEFT_BOTTOM_RIGHT,
    SN_CURSOR_RESIZE_MULTI,

    SN_CURSOR_TEXT_EDIT,

    SN_CURSOR_TYPE_COUNT // Keep last
};

} // namespace sn

#endif // __HEADER_SN_CURSOR__

