#include "Anchors.h"

using namespace sn;

namespace tgui
{

void serializeAnchors(sn::Variant & o, const Anchors & anchors)
{
	if (anchors.all())
	{
		o.setString("fill");
	}
	else
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
	else if (o.isString())
	{
		const std::string & s = o.getString();
		if (s == "fill")
		{
			for (u32 i = 0; i < anchors.size(); ++i)
				anchors[i] = true;
		}
	}
}

void applyAnchors(IntRect & out_rect, const IntRect & container, const Anchors & anchors)
{
    if (anchors[TGUI_LEFT] && anchors[TGUI_RIGHT])
    {
        // Fill horizontal space
        out_rect.width() = container.width();
        out_rect.x() = container.x();
    }
    else
    {
	    if (anchors[TGUI_LEFT])
	    {
            // Align to left
		    out_rect.x() = container.x();
	    }
	    if (anchors[TGUI_RIGHT])
	    {
            // Align to right
            out_rect.x() = container.x() + container.width() - out_rect.width();
	    }
    }

    if (anchors[TGUI_TOP] && anchors[TGUI_BOTTOM])
    {
        // Fill vertical space
        out_rect.height() = container.height();
        out_rect.y() = container.y();
    }
    else
    {
	    if (anchors[TGUI_TOP])
	    {
            // Align to left
		    out_rect.y() = container.y();
	    }
	    if (anchors[TGUI_BOTTOM])
	    {
            // Align to right
            out_rect.y() = container.y() + container.height() - out_rect.height();
	    }
    }

    // Old implementation that applies only on individual border coordinates
    /*
	if (anchors[TGUI_LEFT])
	{
		out_rect.width() += (out_rect.x() - container.x());
		out_rect.x() = container.x();
	}
	if (anchors[TGUI_RIGHT])
	{
		out_rect.width() = (container.x() + container.width()) - out_rect.x();
	}
	if (anchors[TGUI_TOP])
	{
		out_rect.height() += (out_rect.y() - container.y());
		out_rect.y() = container.y();
	}
	if (anchors[TGUI_BOTTOM])
	{
		out_rect.height() = (container.y() + container.height()) - out_rect.y();
	}
    */
}

} // namespace tgui

