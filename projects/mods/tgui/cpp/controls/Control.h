#ifndef __HEADER_TGUI_CONTROL__
#define __HEADER_TGUI_CONTROL__

#include <core/math/Rect.h>
#include <core/scene/Entity.h>
#include <core/scene/base/IDrawContext.h>
#include <core/math/Vector2.h>

#include "../Direction.h"
#include "../theme/Theme.h"
#include "../DrawBatch.h"
#include "../Anchors.h"
#include "../Position.h"

namespace tgui
{

enum ControlFlags
{
    TGUI_CF_ENABLED = 0,
    TGUI_CF_VISIBLE = 1,
    TGUI_CF_HOVERED = 2,
    TGUI_CF_PRESSED = 3,
    TGUI_CF_FOCUSED = 4,
	TGUI_CF_CAPTURED = 5,
    
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
class Layout;

class Control : public sn::Entity
{
public:
    SN_ENTITY(tgui::Control, sn::Entity)

    Control();

    //--------------------------------
    // Geometry
    //--------------------------------

    sn::Vector2i getPosition() const;
    sn::Vector2i getLocalPosition() const { return m_localBounds.origin(); }

    sn::Vector2i getSize() const { return m_localBounds.size(); }

    sn::IntRect getClientBounds() const;

    const sn::IntRect & getLocalClientBounds() const { return m_localBounds; }
    void setLocalClientBounds(sn::IntRect bounds);

    Control * getChildControlAt(sn::Vector2i position) const;

    //--------------------------------
    // Layout
    //--------------------------------
    
    const Anchors & getAnchors() const { return m_anchors; }
    const Border & getMargin() const { return m_margins; }
    const Border & getPadding() const { return m_padding; }
    Position getPositionMode() const { return m_positionMode; }

    virtual void layoutChildren();

    //--------------------------------
    // State
    //--------------------------------

    bool getControlFlag(sn::u32 i) const;

    Control * getParentControl() const;
    const Control * getRootControl() const;

    sn::u32 getWindowID() const;

    virtual void onEvent(Event & ev);

    GUI * getGUI() const;

    void setParent(Entity * newParent) override;

    void setFocus(bool isFocused);

    //--------------------------------
    // Event handlers
    //--------------------------------

	virtual void onReady() override;

    //--------------------------------
    // Helpers
    //--------------------------------

    bool isHovered() const { return getControlFlag(TGUI_CF_HOVERED); }
    bool isPressed() const { return getControlFlag(TGUI_CF_PRESSED); }
    bool isVisible() const { return getControlFlag(TGUI_CF_VISIBLE); }
    bool isFocused() const { return getControlFlag(TGUI_CF_FOCUSED); }

    const Theme * getTheme() const;

    //--------------------------------
    // Serialization
    //--------------------------------

    void serializeState(sn::Variant & o, const sn::SerializationContext & ctx) override;
    void unserializeState(const sn::Variant & o, const sn::SerializationContext & ctx) override;

protected:
    virtual ~Control();

    void dispatchEventToChildren(Event & ev);
    
    virtual void onDraw(DrawBatch & batch);
    virtual void onDrawSelf(DrawBatch & batch) {}

    virtual void onMouseMove(Event & e) {}
    virtual void onMouseEnter(Event & e) {}
    virtual void onMouseLeave(Event & e) {}
    virtual void onMousePress(Event & e) {}
    virtual void onMouseRelease(Event & e) {}
    virtual void onKeyDown(Event & e) {}
    virtual void onControlResized() {}
    virtual void onSetCursor(Event & e);
    virtual void onSizeChanged() {}

    bool hasLayout() const { return m_layout != nullptr; }

	//--------------------------------
    // Helpers
    //--------------------------------

	void beginCapture();
	void endCapture();

private:
    void processMouseMove(Event & e);
    void processMousePress(Event & e);
    void processMouseRelease(Event & e);
    void processKeyDown(Event & e);

    void setControlFlag(sn::u32 i, bool value);

    void setLayout(Layout * newLayout);

private:
    sn::IntRect m_localBounds;
    std::bitset<TGUI_CF_COUNT> m_controlFlags;
    sn::u32 m_windowID;
    Border m_margins;
    Border m_padding;
    Anchors m_anchors;
    Position m_positionMode;
    Layout * m_layout; // TODO Layout should be a Component in future design
};

} // namespace tgui

#endif // __HEADER_TGUI_CONTROL__

