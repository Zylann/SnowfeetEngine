#ifndef __HEADER_SN_RENDERSTATE__
#define __HEADER_SN_RENDERSTATE__

#include <core/math/Matrix4.h>
#include <modules/render/VideoDriver.h>

namespace sn {
namespace render {

/// \brief Additional data carried during rendering of entities
class RenderState
{
public:
    RenderState(VideoDriver & a_driver) :
        driver(a_driver)
    {}

    VideoDriver & driver;
    Matrix4 viewMatrix;
    Matrix4 projectionMatrix;
    Matrix4 normalMatrix;

};

} // namespace render
} // namespace sn

#endif // __HEADER_SN_RENDERSTATE__

