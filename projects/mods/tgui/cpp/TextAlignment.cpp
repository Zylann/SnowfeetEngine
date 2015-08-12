#include "TextAlignment.h"

namespace tgui
{

void serialize(JsonBox::Value & o, TextAlignment align)
{
    switch (align)
    {
    case TGUI_ALIGN_LEFT: o = "left";
    case TGUI_ALIGN_CENTER: o = "center";
    case TGUI_ALIGN_RIGHT: o = "right";
    default: break;
    }
}

void unserialize(const sn::Variant & o, TextAlignment & align)
{
    const std::string & str = o.getString();
    if (str == "left")
        align = TGUI_ALIGN_LEFT;
    else if (str == "center")
        align = TGUI_ALIGN_CENTER;
    else if (str == "right")
        align = TGUI_ALIGN_RIGHT;
}

} // namespace tgui

