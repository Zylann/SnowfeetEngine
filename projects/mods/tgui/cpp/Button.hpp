#ifndef __HEADER_TGUI_BUTTON__
#define __HEADER_TGUI_BUTTON__

#include "Control.hpp"
#include <core/scene/base/IDrawContext.hpp>

namespace tgui
{

class Button : public Control
{
public:
    SN_ENTITY(tgui::Button, tgui::Control)

    virtual void onMouseEnter(Event & e) override;
    virtual void onMouseLeave(Event & e) override;
    virtual void onMousePress(Event & e) override;
    virtual void onMouseRelease(Event & e) override;

    virtual void onPress() {}
    virtual void onRelease() {}

    void onDrawSelf(sn::IDrawContext & dc) override;

    //--------------------------------
    // Serialization
    //--------------------------------

    void serializeState(JsonBox::Value & o, const sn::SerializationContext & ctx) override;
    void unserializeState(JsonBox::Value & o, const sn::SerializationContext & ctx) override;

};

} // namespace tgui

#endif // __HEADER_TGUI_BUTTON__

