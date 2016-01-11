#include "Position.h"
#include <core/util/Log.h>

namespace tgui
{

void serialize(sn::Variant & o, Position p)
{
    switch (p)
    {
    case TGUI_LAYOUT: o = "layout"; break;
    case TGUI_RELATIVE: o = "relative"; break;
    default: break;
    }
}

void unserialize(const sn::Variant & o, Position & p)
{
    const std::string & s = o.getString();
    if (s == "layout")
        p = TGUI_LAYOUT;
    else if (s == "relative")
        p = TGUI_RELATIVE;
    else
        SN_WARNING("Unknown Position value: '" + s + '\'');
}

} // namespace tgui

