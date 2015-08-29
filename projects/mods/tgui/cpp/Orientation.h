#ifndef __HEADER_TGUI_ORIENTATION__
#define __HEADER_TGUI_ORIENTATION__

#include <core/sml/variant_serialize.h>

namespace tgui
{

enum Orientation
{
    TGUI_HORIZONTAL = 0,
    TGUI_VERTICAL
};

inline void serialize(sn::Variant & o, Orientation v)
{
    if (v == TGUI_VERTICAL)
        o = "vertical";
    else
        o = "horizontal";
}

inline void unserialize(const sn::Variant & o, Orientation v)
{
    const std::string & s = o.getString();
    if (s == "vertical")
        v = TGUI_VERTICAL;
    else
        v = TGUI_HORIZONTAL;
}

} // namespace tgui

#endif // __HEADER_TGUI_ORIENTATION__

