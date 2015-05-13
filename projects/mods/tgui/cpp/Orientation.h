#ifndef __HEADER_TGUI_ORIENTATION__
#define __HEADER_TGUI_ORIENTATION__

#include <core/json/json_utils.hpp>

namespace tgui
{

enum Orientation
{
    TGUI_HORIZONTAL = 0,
    TGUI_VERTICAL    
};

inline void serialize(JsonBox::Value & o, Orientation v)
{
    if (v == TGUI_VERTICAL)
        o = "vertical";
    else
        o = "horizontal";
}

inline void unserialize(JsonBox::Value & o, Orientation v)
{
    const std::string & s = o.getString();
    if (s == "vertical")
        v = TGUI_VERTICAL;
    else
        v = TGUI_HORIZONTAL;
}

} // namespace tgui

#endif // __HEADER_TGUI_ORIENTATION__

