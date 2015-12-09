#ifndef __HEADER_SNR_BLENDMODE__
#define __HEADER_SNR_BLENDMODE__

#include <core/sml/variant_serialize.h>
#include <modules/render/common.h>

namespace sn
{

enum BlendMode
{
    SNR_BLEND_NONE = 0,
    SNR_BLEND_ALPHA
};

void SN_RENDER_API serialize(sn::Variant & o, BlendMode m);
void SN_RENDER_API unserialize(const sn::Variant & o, BlendMode & m);

} // namespace sn

#endif // __HEADER_SNR_BLENDMODE__

