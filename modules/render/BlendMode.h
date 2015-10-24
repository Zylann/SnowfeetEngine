#ifndef __HEADER_SNR_BLENDMODE__
#define __HEADER_SNR_BLENDMODE__

#include <core/sml/variant_serialize.h>

namespace sn {
namespace render {

enum BlendMode
{
    SNR_BLEND_NONE = 0,
    SNR_BLEND_ALPHA
};

void serialize(sn::Variant & o, BlendMode m);
void unserialize(const sn::Variant & o, BlendMode & m);

} // namespace render
} // namespace sn

#endif // __HEADER_SNR_BLENDMODE__

