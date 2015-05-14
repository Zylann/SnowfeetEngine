#ifndef __HEADER_TGUI_POSITION__
#define __HEADER_TGUI_POSITION__

#include <core/json/json_utils.hpp>

namespace tgui
{

enum Position
{
    //TGUI_ABSOLUTE = 0,
    TGUI_RELATIVE = 1,
    TGUI_LAYOUT = 2
};

void serialize(JsonBox::Value & o, Position p);
void unserialize(JsonBox::Value & o, Position & p);

} // namespace tgui

#endif // __HEADER_TGUI_POSITION__

