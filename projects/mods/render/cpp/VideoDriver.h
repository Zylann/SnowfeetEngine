#ifndef __HEADER_SNR_VIDEODRIVER__
#define __HEADER_SNR_VIDEODRIVER__

#include <core/drivers/VideoDriver.h>

namespace sn {
namespace render {

class VideoDriver : public IVideoDriver
{
public:
    SN_OBJECT(sn::render::VideoDriver, sn::IVideoDriver)

    sn::TextureBase * createTexture() override;

};

} // namespace render
} // namespace sn

#endif // __HEADER_SNR_VIDEODRIVER__

