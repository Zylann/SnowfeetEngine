#ifndef __HEADER_TGUI_BORDER__
#define __HEADER_TGUI_BORDER__

#include <core/sml/variant_serialize.h>

namespace tgui
{

/// \brief Distance to each border of a rectangle
struct Border
{
    sn::s32 left;
    sn::s32 right;
    sn::s32 top;
    sn::s32 bottom;

    Border() { memset(this, 0, sizeof(Border)); }

	void crop(sn::IntRect & inout_r) const
	{
		inout_r.x() += left;
		inout_r.y() += top;
		inout_r.width() -= (left + right);
		inout_r.height() -= (top + bottom);
	}

	void expand(sn::IntRect & inout_r) const
	{
		inout_r.x() -= left;
		inout_r.y() -= top;
		inout_r.width() += (left + right);
		inout_r.height() += (top + bottom);
	}
};

inline void serialize(sn::Variant & o, const Border & b)
{
    sn::serialize(o["left"], b.left);
    sn::serialize(o["right"], b.right);
    sn::serialize(o["top"], b.top);
    sn::serialize(o["bottom"], b.bottom);
}

inline void unserialize(const sn::Variant & o, Border & b)
{
    sn::unserialize(o["left"], b.left);
    sn::unserialize(o["right"], b.right);
    sn::unserialize(o["top"], b.top);
    sn::unserialize(o["bottom"], b.bottom);
}

} // namespace tgui

#endif // __HEADER_TGUI_BORDER__
