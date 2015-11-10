#ifndef __HEADER_TGUI_RENDERER__
#define __HEADER_TGUI_RENDERER__

#include <modules/render/entities/Drawable.h>

namespace tgui
{

class Renderer : public sn::render::Drawable
{
public:
    SN_ENTITY(tgui::Renderer, sn::render::Drawable)

protected:
    void onDraw(sn::render::IDrawContext & dc) override;
};

} // namespace tgui

#endif // __HEADER_TGUI_RENDERER__

