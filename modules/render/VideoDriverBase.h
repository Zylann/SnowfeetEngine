#ifndef __HEADER_SN_VIDEODRIVER__
#define __HEADER_SN_VIDEODRIVER__

#include <core/drivers/Driver.h>

#include <modules/render/Texture.h>
#include <modules/render/common.h>

namespace sn
{

// TODO graphics API-specific functionnality should go here

/// \brief Access to accelerated graphics functions
class SN_RENDER_API IVideoDriver : public IDriver
{
public:
    SN_OBJECT(sn::IVideoDriver, sn::IDriver);

    virtual sn::render::Texture * createTexture() = 0;

};

} // namespace sn

#endif // __HEADER_SN_VIDEODRIVER__

