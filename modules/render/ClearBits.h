#ifndef __HEADER_SNR_CLEARBITS__
#define __HEADER_SNR_CLEARBITS__

#include <core/sml/variant_serialize.h>
#include <modules/render/common.h>

namespace sn {
namespace render {

enum ClearBits
{
    SNR_CLEAR_NONE = 0,
    
    SNR_CLEAR_COLOR = 1,
    SNR_CLEAR_DEPTH = 2,
    //SNR_CLEAR_SKYBOX,

    SNR_CLEAR_ALL = SNR_CLEAR_COLOR | SNR_CLEAR_DEPTH
};

typedef u32 ClearMask;

void SN_RENDER_API serialize(sn::Variant & o, ClearMask m);
void SN_RENDER_API unserialize(const sn::Variant & o, ClearMask & m);

} // namespace render
} // namespace sn

#endif // __HEADER_SNR_CLEARBITS__

