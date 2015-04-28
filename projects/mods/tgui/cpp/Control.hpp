#ifndef __HEADER_TGUI_CONTROL__
#define __HEADER_TGUI_CONTROL__

#include <core/math/Rect.hpp>
#include <core/scene/Entity.hpp>

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
        m_controlFlags(TGUI_ENABLED | TGUI_VISIBLE)
    {}

    sn::IntRect getClientBounds() const;
    void setClientBounds(sn::IntRect bounds);

    bool getControlFlag(sn::u32 i);
    void setControlFlag(sn::u32 i, bool value);

    Control * getParentControl() const;

    virtual void onEvent(Event & ev);

    virtual void onMouseMove(Event & e) {}
    virtual void onMouseEnter(Event & e) {}
    virtual void onMouseLeave(Event & e) {}
    virtual void onMousePress(Event & e) {}
    virtual void onMouseRelease(Event & e) {}
    virtual void onControlResized() {}

    GUI * getGUI() const;

protected:
    void dispatchEventToChildren(Event & ev);

private:
    sn::IntRect m_clientBounds;
    std::bitset<8> m_controlFlags;

};

} // namespace tgui

#endif // __HEADER_TGUI_CONTROL__

