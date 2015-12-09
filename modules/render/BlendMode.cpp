#include "BlendMode.h"

namespace sn
{

void serialize(sn::Variant & o, BlendMode m)
{
    switch (m)
    {
    case SNR_BLEND_ALPHA: o = "alpha";
    default: o = "none";
    }
}

void unserialize(const sn::Variant & o, BlendMode & m)
{
    const std::string & s = o.getString();
    if (s == "alpha")
        m = SNR_BLEND_ALPHA;
    else
        m = SNR_BLEND_NONE;
}

} // namespace sn

