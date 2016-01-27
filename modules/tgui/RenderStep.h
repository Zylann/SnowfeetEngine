#ifndef __HEADER_TGUI_RENDERSTEP__
#define __HEADER_TGUI_RENDERSTEP__

#include <modules/render/entities/RenderStep.h>
#include <modules/tgui/common.h>

namespace tgui
{

class SN_TGUI_API RenderStep : public sn::RenderStep
{
public:
    SN_OBJECT

    void onDraw(sn::VideoDriver & driver) override;

};

} // namespace tgui

#endif // __HEADER_TGUI_RENDERSTEP__

