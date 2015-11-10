#ifndef __HEADER_SN_DRAWABLE__
#define __HEADER_SN_DRAWABLE__

#include <core/scene/Entity3D.h>

#include <modules/render/IDrawContext.h>
#include <modules/render/common.h>

namespace sn
{

/// \brief Entity having a visual appearance
class SN_RENDER_API Drawable : public Entity3D
{
public:
    SN_ENTITY(sn::Drawable, sn::Entity3D)

    static const std::string TAG;

    void onReady() override;
    virtual void onDraw(IDrawContext & context) = 0;

};

} // namespace sn

#endif // __HEADER_SN_DRAWABLE__


