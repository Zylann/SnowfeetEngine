#ifndef __HEADER_TGUI_POSITION__
#define __HEADER_TGUI_POSITION__

#include <core/sml/variant_serialize.h>
#include <modules/tgui/common.h>

namespace tgui
{

enum Position
{
    //TGUI_ABSOLUTE = 0,
    TGUI_RELATIVE = 1,
    TGUI_LAYOUT = 2
};

void SN_TGUI_API serialize(sn::Variant & o, Position p);
void SN_TGUI_API unserialize(const sn::Variant & o, Position & p);

} // namespace tgui

#endif // __HEADER_TGUI_POSITION__

