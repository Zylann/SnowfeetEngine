#ifndef __HEADER_TGUI_TEXTALIGNMENT__
#define __HEADER_TGUI_TEXTALIGNMENT__

#include <core/sml/variant_serialize.h>
#include <modules/tgui/common.h>

namespace tgui
{

enum TextAlignment
{
    TGUI_ALIGN_LEFT,
    TGUI_ALIGN_CENTER,
    TGUI_ALIGN_RIGHT
};

void SN_TGUI_API serialize(sn::Variant & o, TextAlignment align);
void SN_TGUI_API unserialize(const sn::Variant & o, TextAlignment & align);

} // namespace tgui

#endif // __HEADER_TGUI_TEXTALIGNMENT__

