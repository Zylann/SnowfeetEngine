#ifndef __HEADER_SN_RENDER_DRAWABLE__
#define __HEADER_SN_RENDER_DRAWABLE__

#include <core/scene/Entity3D.h>

#include <modules/render/RenderState.h>
#include <modules/render/common.h>

namespace sn
{

/// \brief Entity having a visual appearance
class SN_RENDER_API Drawable : public Entity3D
{
public:
    SN_OBJECT(sn::Drawable, sn::Entity3D)

    static const std::string TAG;

    void onReady() override;
    virtual void onDraw(RenderState & state) = 0;

};

} // namespace sn

#endif // __HEADER_SN_RENDER_DRAWABLE__


