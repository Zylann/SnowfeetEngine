#ifndef __HEADER_SN_VIDEODRIVER__
#define __HEADER_SN_VIDEODRIVER__

#include <core/asset/base/TextureBase.hpp>
#include <core/drivers/Driver.hpp>

namespace sn
{

// TODO graphics API-specific functionnality should go here

/// \brief Access to accelerated graphics functions
class SN_API IVideoDriver : public IDriver
{
public:
    SN_OBJECT(sn::IVideoDriver, sn::IDriver);

    virtual sn::TextureBase * createTexture() = 0;

};

} // namespace sn

#endif // __HEADER_SN_VIDEODRIVER__

