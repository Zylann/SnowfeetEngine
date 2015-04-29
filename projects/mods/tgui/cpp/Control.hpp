#ifndef __HEADER_TGUI_CONTROL__
#define __HEADER_TGUI_CONTROL__

#include <core/math/Rect.hpp>
#include <core/scene/Entity.hpp>
#include <core/scene/base/IDrawContext.hpp>
#include <core/math/Vector2.hpp>

namespace tgui
{

enum WidgetFlags
{
    TGUI_ENABLED = 1,
    TGUI_VISIBLE = 2,
    TGUI_HOVERED = 4,
    TGUI_PRESSED = 8,
    TGUI_FOCUSED = 16
};

struct Event
{
    sn::Event value;
    bool consumed;

    Event() : consumed(false) {}

    void consume() { consumed = true; }
};

class GUI;

class Control : public sn::Entity
{
public:
    SN_ENTITY(tgui::Control, sn::Entity)

    Control() :
        m_controlFlags(TGUI_ENABLED | TGUI_VISIBLE),
        m_windowID(0)
    {}

    sn::IntRect getClientBounds() const;
    void setClientBounds(sn::IntRect bounds);

    bool getControlFlag(sn::u32 i) const;
    void setControlFlag(sn::u32 i, bool value);

    Control * getParentControl() const;
    const Control * getRootControl() const;

    sn::u32 getWindowID() const;

    Control * getChildControlAt(sn::Vector2i position) const;

    virtual void onEvent(Event & ev);

    virtual void onMouseMove(Event & e) {}
    virtual void onMouseEnter(Event & e) {}
    virtual void onMouseLeave(Event & e) {}
    virtual void onMousePress(Event & e) {}
    virtual void onMouseRelease(Event & e) {}
    virtual void onControlResized() {}

    GUI * getGUI() const;

    //--------------------------------
    // Serialization
    //--------------------------------

    void serializeState(JsonBox::Value & o, const sn::SerializationContext & ctx) override;
    void unserializeState(JsonBox::Value & o, const sn::SerializationContext & ctx) override;

protected:
    void dispatchEventToChildren(Event & ev);

    virtual void onDraw(sn::IDrawContext & dc);
    virtual void onDrawSelf(sn::IDrawContext & dc) {}

private:
    sn::IntRect m_clientBounds;
    std::bitset<8> m_controlFlags;
    sn::u32 m_windowID;

};

} // namespace tgui

#endif // __HEADER_TGUI_CONTROL__

