#ifndef __HEADER_SNR_VIDEODRIVER__
#define __HEADER_SNR_VIDEODRIVER__

#include <core/drivers/Driver.h>
#include <modules/render/Texture.h>

namespace sn {
namespace render {

class SN_RENDER_API VideoDriver : public sn::IDriver
{
public:
    SN_OBJECT(sn::render::VideoDriver, sn::IDriver)

    sn::render::Texture * createTexture();

};

} // namespace render
} // namespace sn

#endif // __HEADER_SNR_VIDEODRIVER__

