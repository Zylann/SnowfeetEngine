#ifndef __HEADER_TGUI_GUI__
#define __HEADER_TGUI_GUI__

#include <core/scene/Drawable.hpp>
#include <core/scene/base/IDrawContext.hpp>

#include "Control.hpp"

namespace tgui
{

class GUI : public Control
{
public:
    SN_ENTITY(tgui::GUI, sn::Drawable)

    void onReady() override;
    void onDraw(sn::IDrawContext & dc);

protected:
    bool onSystemEvent(const sn::Event & systemEvent) override;

};

} // namespace tgui

#endif // __HEADER_TGUI_GUI__

