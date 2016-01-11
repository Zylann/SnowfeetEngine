#ifndef __HEADER_TGUI_ANCHORS__
#define __HEADER_TGUI_ANCHORS__

#include <core/sml/variant_serialize.h>
#include <core/math/Rect.h>
#include "Direction.h"
#include <bitset>

namespace tgui
{

typedef std::bitset<TGUI_DIRECTION_COUNT> Anchors;

void serializeAnchors(sn::Variant & o, const Anchors & anchors);
void unserializeAnchors(const sn::Variant & o, Anchors & anchors);

void applyAnchors(sn::IntRect & out_rect, const sn::IntRect & container, const Anchors & anchors);

} // namespace tgui

#endif // __HEADER_TGUI_ANCHORS__

