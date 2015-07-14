#include "BlendMode.h"

namespace sn {
namespace render {

void serialize(JsonBox::Value & o, BlendMode m)
{
    switch (m)
    {
    case SNR_BLEND_ALPHA: o = "alpha";
    default: o = "none";
    }
}

void unserialize(JsonBox::Value & o, BlendMode & m)
{
    const std::string & s = o.getString();
    if (s == "alpha")
        m = SNR_BLEND_ALPHA;
    else
        m = SNR_BLEND_NONE;
}

} // namespace render
} // namespace sn

