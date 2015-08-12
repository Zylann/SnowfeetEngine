#include "Anchors.h"

using namespace sn;

namespace tgui
{

void serializeAnchors(JsonBox::Value & o, const Anchors & anchors)
{
    JsonBox::Array a;
    for (u32 d = 0; d < TGUI_DIRECTION_COUNT; ++d)
    {
        if (anchors[d])
        {
            JsonBox::Value v;
            serialize(v, (Direction)d);
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
            unserialize(o[i], d);
            if (d != TGUI_DIRECTION_COUNT)
            {
                anchors[d] = true;
            }
        }
    }
}

} // namespace tgui

