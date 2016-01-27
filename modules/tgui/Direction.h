#ifndef __HEADER_TGUI_DIRECTION__
#define __HEADER_TGUI_DIRECTION__

#include <core/sml/variant_serialize.h>
#include <modules/tgui/common.h>

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

void SN_TGUI_API serialize(sn::Variant & o, Direction d);
void SN_TGUI_API unserialize(const sn::Variant & o, Direction & d);

} // namespace tgui

#endif // __HEADER_TGUI_DIRECTION__

