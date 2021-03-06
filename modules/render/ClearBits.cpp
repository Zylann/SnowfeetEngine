#include "ClearBits.h"

namespace sn
{

//------------------------------------------------------------------------------
void serializeClearMask(sn::Variant & o, ClearMask m)
{
    sn::Variant::Array a;
    if (m & SNR_CLEAR_ALL)
        a.push_back("all");
    else
    {
        if (m & SNR_CLEAR_COLOR)
            a.push_back("color");
        if (m & SNR_CLEAR_DEPTH)
            a.push_back("depth");
    }
    o = a;
}

//------------------------------------------------------------------------------
void unserializeClearMask(const sn::Variant & o, ClearMask & m)
{
    if (!o.isArray())
        return;
    const sn::Variant::Array & a = o.getArray();
    for (u32 i = 0; i < a.size(); ++i)
    {
        const std::string & s = o[i].getString();
        if (s == "all")
        {
            m = SNR_CLEAR_ALL;
            break;
        }
        else
        {
            if (s == "color")
                m |= SNR_CLEAR_COLOR;
            else if (s == "depth")
                m |= SNR_CLEAR_DEPTH;
            else if (s == "none")
                m = SNR_CLEAR_NONE;
            else
                SN_WARNING("Unknown ClearMask value: '" << s << "'");
        }
    }
}

} // namespace sn


