#ifndef __HEADER_TGUI_TEXTALIGNMENT__
#define __HEADER_TGUI_TEXTALIGNMENT__

#include <core/json/json_utils.h>

namespace tgui
{

enum TextAlignment
{
    TGUI_ALIGN_LEFT,
    TGUI_ALIGN_CENTER,
    TGUI_ALIGN_RIGHT
};

void serialize(JsonBox::Value & o, TextAlignment align);
void unserialize(JsonBox::Value & o, TextAlignment & align);

} // namespace tgui

#endif // __HEADER_TGUI_TEXTALIGNMENT__

