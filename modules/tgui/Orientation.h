#ifndef __HEADER_TGUI_ORIENTATION__
#define __HEADER_TGUI_ORIENTATION__

#include <core/sml/variant_serialize.h>

namespace tgui
{

/// \brief Orientation indexes. Can also be used to index X or Y property on vectors.
enum Orientation
{
    TGUI_HORIZONTAL = 0, // X
    TGUI_VERTICAL // Y
};

inline Orientation opposite(Orientation o)
{
	return o == TGUI_HORIZONTAL ? TGUI_VERTICAL : TGUI_HORIZONTAL;
}

inline void serialize(sn::Variant & o, Orientation v)
{
    if (v == TGUI_VERTICAL)
        o = "vertical";
    else
        o = "horizontal";
}

inline void unserialize(const sn::Variant & o, Orientation & v)
{
    const std::string & s = o.getString();
    if (s == "vertical")
        v = TGUI_VERTICAL;
    else
        v = TGUI_HORIZONTAL;
}

} // namespace tgui

#endif // __HEADER_TGUI_ORIENTATION__

