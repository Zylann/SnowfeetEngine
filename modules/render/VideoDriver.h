#ifndef __HEADER_SNR_VIDEODRIVER__
#define __HEADER_SNR_VIDEODRIVER__

#include <modules/render/VideoDriverBase.h>

namespace sn {
namespace render {

class VideoDriver : public IVideoDriver
{
public:
    SN_OBJECT(sn::render::VideoDriver, sn::IVideoDriver)

    sn::render::Texture * createTexture() override;

};

} // namespace render
} // namespace sn

#endif // __HEADER_SNR_VIDEODRIVER__

