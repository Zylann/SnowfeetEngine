#ifndef __HEADER_SNR_BLENDMODE__
#define __HEADER_SNR_BLENDMODE__

#include <core/json/json_utils.hpp>

namespace sn {
namespace render {

enum BlendMode
{
    SNR_BLEND_NONE = 0,
    SNR_BLEND_ALPHA
};

void serialize(JsonBox::Value & o, BlendMode m);
void unserialize(JsonBox::Value & o, BlendMode & m);

} // namespace render
} // namespace sn

#endif // __HEADER_SNR_BLENDMODE__

