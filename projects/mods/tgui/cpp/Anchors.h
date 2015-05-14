#ifndef __HEADER_TGUI_ANCHORS__
#define __HEADER_TGUI_ANCHORS__

#include "Direction.h"
#include <bitset>

namespace tgui
{

typedef std::bitset<TGUI_DIRECTION_COUNT> Anchors;

void serializeAnchors(JsonBox::Value & o, const Anchors & anchors);
void unserializeAnchors(JsonBox::Value & o, Anchors & anchors);

} // namespace tgui

#endif // __HEADER_TGUI_ANCHORS__

