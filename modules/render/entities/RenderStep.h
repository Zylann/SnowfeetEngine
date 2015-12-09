#ifndef __HEADER_SN_RENDER_STEP__
#define __HEADER_SN_RENDER_STEP__

#include <core/scene/Entity.h>
#include <modules/render/entities/RenderManager.h>

namespace sn
{

class SN_RENDER_API RenderStep : public sn::Entity
{
public:
    SN_ENTITY(sn::RenderStep, sn::Entity)

    RenderStep();

    virtual void onDraw(VideoDriver & driver) = 0;

protected:
    RenderManager * getManager() const;

private:
    mutable RenderManager * r_manager;

};

} // namespace sn

#endif // __HEADER_SN_RENDER_STEP__

