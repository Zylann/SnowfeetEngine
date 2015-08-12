#include "Anchors.h"

using namespace sn;

namespace tgui
{

void serializeAnchors(sn::Variant & o, const Anchors & anchors)
{
    Variant::Array a;
    for (u32 d = 0; d < TGUI_DIRECTION_COUNT; ++d)
    {
        if (anchors[d])
        {
            Variant v;
            tgui::serialize(v, (Direction)d);
            a.push_back(v);
        }
    }
    o = a;
}

void unserializeAnchors(const sn::Variant & o, Anchors & anchors)
{
    if (o.isArray())
    {
        const Variant::Array & a = o.getArray();
        for (u32 i = 0; i < a.size(); ++i)
        {
            Direction d = TGUI_DIRECTION_COUNT;
            tgui::unserialize(o[i], d);
            if (d != TGUI_DIRECTION_COUNT)
            {
                anchors[d] = true;
            }
        }
    }
}

} // namespace tgui

