#ifndef __HEADER_TGUI_ANCHORS__
#define __HEADER_TGUI_ANCHORS__

#include <core/sml/variant_serialize.h>
#include "Direction.h"
#include <bitset>

namespace tgui
{

typedef std::bitset<TGUI_DIRECTION_COUNT> Anchors;

void serializeAnchors(JsonBox::Value & o, const Anchors & anchors);
void unserializeAnchors(const sn::Variant & o, Anchors & anchors);

} // namespace tgui

#endif // __HEADER_TGUI_ANCHORS__

