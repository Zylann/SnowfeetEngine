#ifndef __HEADER_TGUI_DIRECTION__
#define __HEADER_TGUI_DIRECTION__

#include <core/json/json_serialize.h>
#include <core/sml/variant_serialize.h>

namespace tgui
{

enum Direction
{
    TGUI_RIGHT = 0,
    TGUI_TOP,
    TGUI_LEFT,
    TGUI_BOTTOM,

    TGUI_DIRECTION_COUNT
};

void serialize(sn::Variant & o, Direction d);
void unserialize(const sn::Variant & o, Direction & d);

} // namespace tgui

#endif // __HEADER_TGUI_DIRECTION__

