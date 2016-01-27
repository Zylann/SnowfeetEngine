#ifndef __HEADER_TGUI_ANCHORS__
#define __HEADER_TGUI_ANCHORS__

#include <core/sml/variant_serialize.h>
#include <core/math/Rect.h>

#include <bitset>

#include <modules/tgui/Direction.h>

namespace tgui
{

typedef std::bitset<TGUI_DIRECTION_COUNT> Anchors;

void SN_TGUI_API serializeAnchors(sn::Variant & o, const Anchors & anchors);
void SN_TGUI_API unserializeAnchors(const sn::Variant & o, Anchors & anchors);

void SN_TGUI_API applyAnchors(sn::IntRect & out_rect, const sn::IntRect & container, const Anchors & anchors);

} // namespace tgui

#endif // __HEADER_TGUI_ANCHORS__

