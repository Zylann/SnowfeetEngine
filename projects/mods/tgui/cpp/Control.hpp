#ifndef __HEADER_TGUI_CONTROL__
#define __HEADER_TGUI_CONTROL__

#include <core/math/Rect.hpp>
#include <core/scene/Entity.hpp>
#include <core/scene/base/IDrawContext.hpp>
#include <core/math/Vector2.hpp>

namespace tgui
{

enum ControlFlags
{
    TGUI_CF_ENABLED = 0,
    TGUI_CF_VISIBLE = 1,
    TGUI_CF_HOVERED = 2,
    TGUI_CF_PRESSED = 3,
    TGUI_CF_FOCUSED = 4,
    
    TGUI_CF_COUNT = 8
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
        m_controlFlags(TGUI_CF_ENABLED | TGUI_CF_VISIBLE),
        m_windowID(0)
    {}

    sn::IntRect getClientBounds() const;
    void setClientBounds(sn::IntRect bounds);

    bool getControlFlag(sn::u32 i) const;

    Control * getParentControl() const;
    const Control * getRootControl() const;

    sn::u32 getWindowID() const;

    Control * getChildControlAt(sn::Vector2i position) const;

    virtual void onEvent(Event & ev);

    GUI * getGUI() const;

    //--------------------------------
    // Helpers
    //--------------------------------

    bool isHovered() const { return getControlFlag(TGUI_CF_HOVERED); }
    bool isPressed() const { return getControlFlag(TGUI_CF_PRESSED); }

    //--------------------------------
    // Serialization
    //--------------------------------

    void serializeState(JsonBox::Value & o, const sn::SerializationContext & ctx) override;
    void unserializeState(JsonBox::Value & o, const sn::SerializationContext & ctx) override;

protected:
    void dispatchEventToChildren(Event & ev);
    
    void setControlFlag(sn::u32 i, bool value);

    virtual void onDraw(sn::IDrawContext & dc);
    virtual void onDrawSelf(sn::IDrawContext & dc) {}

    virtual void onMouseMove(Event & e) {}
    virtual void onMouseEnter(Event & e) {}
    virtual void onMouseLeave(Event & e) {}
    virtual void onMousePress(Event & e) {}
    virtual void onMouseRelease(Event & e) {}
    virtual void onControlResized() {}

private:
    void processMouseMove(Event & e);
    void processMousePress(Event & e);
    void processMouseRelease(Event & e);

private:
    sn::IntRect m_clientBounds;
    std::bitset<TGUI_CF_COUNT> m_controlFlags;
    sn::u32 m_windowID;

};

} // namespace tgui

#endif // __HEADER_TGUI_CONTROL__

