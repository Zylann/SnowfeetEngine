#include "Direction.h"

namespace tgui
{

void serialize(JsonBox::Value & o, Direction d)
{
    switch (d)
    {
    case TGUI_LEFT: o = "left"; break;
    case TGUI_RIGHT: o = "right"; break;
    case TGUI_TOP: o = "top"; break;
    case TGUI_BOTTOM: o = "bottom"; break;
    default: break;
    }
}

void unserialize(JsonBox::Value & o, Direction & d)
{
    const std::string & s = o.getString();
    if (s == "left")
        d = TGUI_LEFT;
    else if (s == "right")
        d = TGUI_RIGHT;
    else if (s == "top")
        d = TGUI_TOP;
    else if (s == "bottom")
        d = TGUI_BOTTOM;
    else
        SN_WARNING("Unknown tgui::Direction value '" + s + '\'');
}

} // namespace tgui

