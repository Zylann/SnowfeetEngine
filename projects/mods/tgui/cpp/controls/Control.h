#ifndef __HEADER_TGUI_CONTROL__
#define __HEADER_TGUI_CONTROL__

#include <core/math/Rect.hpp>
#include <core/scene/Entity.hpp>
#include <core/scene/base/IDrawContext.hpp>
#include <core/math/Vector2.hpp>

#include "../theme/Theme.h"
#include "../DrawBatch.h"

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

    //--------------------------------
    // Geometry
    //--------------------------------

    sn::Vector2i getPosition() const;
    sn::Vector2i getLocalPosition() const { return m_localBounds.origin(); }

    sn::Vector2i getSize() const { return m_localBounds.size(); }

    sn::IntRect getClientBounds() const;
    void setLocalClientBounds(sn::IntRect bounds);

    Control * getChildControlAt(sn::Vector2i position) const;

    //--------------------------------
    // State
    //--------------------------------

    bool getControlFlag(sn::u32 i) const;

    Control * getParentControl() const;
    const Control * getRootControl() const;

    sn::u32 getWindowID() const;

    virtual void onEvent(Event & ev);

    GUI * getGUI() const;

    //--------------------------------
    // Helpers
    //--------------------------------

    bool isHovered() const { return getControlFlag(TGUI_CF_HOVERED); }
    bool isPressed() const { return getControlFlag(TGUI_CF_PRESSED); }

    const Theme * getTheme() const;

    //--------------------------------
    // Serialization
    //--------------------------------

    void serializeState(JsonBox::Value & o, const sn::SerializationContext & ctx) override;
    void unserializeState(JsonBox::Value & o, const sn::SerializationContext & ctx) override;

protected:
    void dispatchEventToChildren(Event & ev);
    
    void setControlFlag(sn::u32 i, bool value);

    virtual void onDraw(DrawBatch & batch);
    virtual void onDrawSelf(DrawBatch & batch) {}

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
    sn::IntRect m_localBounds;
    std::bitset<TGUI_CF_COUNT> m_controlFlags;
    sn::u32 m_windowID;

};

} // namespace tgui

#endif // __HEADER_TGUI_CONTROL__

