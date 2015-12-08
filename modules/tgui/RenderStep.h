#ifndef __HEADER_TGUI_RENDERSTEP__
#define __HEADER_TGUI_RENDERSTEP__

#include <modules/render/entities/RenderStep.h>

namespace tgui
{

class RenderStep : public sn::render::RenderStep
{
public:
    SN_ENTITY(tgui::RenderStep, sn::render::RenderStep)

    void onDraw(sn::render::VideoDriver & driver) override;

};

} // namespace tgui

#endif // __HEADER_TGUI_RENDERSTEP__

