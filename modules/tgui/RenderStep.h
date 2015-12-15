#ifndef __HEADER_TGUI_RENDERSTEP__
#define __HEADER_TGUI_RENDERSTEP__

#include <modules/render/entities/RenderStep.h>

namespace tgui
{

class RenderStep : public sn::RenderStep
{
public:
    SN_OBJECT

    void onDraw(sn::VideoDriver & driver) override;

};

} // namespace tgui

#endif // __HEADER_TGUI_RENDERSTEP__

