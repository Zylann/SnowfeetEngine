#ifndef __HEADER_TGUI_GUI__
#define __HEADER_TGUI_GUI__

#include <core/scene/Drawable.hpp>
#include <core/scene/base/IDrawContext.hpp>
#include <core/util/SharedRef.hpp>

#include "Control.hpp"
#include "Theme.hpp"

namespace tgui
{

class GUI : public Control
{
public:
    SN_ENTITY(tgui::GUI, sn::Drawable)

    void draw(sn::IDrawContext & dc);

    void onReady() override;

    const Theme & getTheme() const;

    //--------------------------------
    // Serialization
    //--------------------------------

    void serializeState(JsonBox::Value & o, const sn::SerializationContext & ctx) override;
    void unserializeState(JsonBox::Value & o, const sn::SerializationContext & ctx) override;

protected:
    bool onSystemEvent(const sn::Event & systemEvent) override;

private:
    Theme m_defaultTheme;
    sn::SharedRef<Theme> m_theme;

};

} // namespace tgui

#endif // __HEADER_TGUI_GUI__

