#include "Panel.hpp"

using namespace sn;

namespace tgui
{

//------------------------------------------------------------------------------
Panel::Panel() : Control(),
    m_isResizeable(false),
    m_sideResizerSize(4), // Default
    m_cornerResizerSize(8) // Default
{
    m_isResizeable = true;
}

//------------------------------------------------------------------------------
void Panel::onDrawSelf(DrawBatch & batch)
{
    const Theme * theme = getTheme();
    if (theme == nullptr)
        return;

    const ControlTheme & ct = theme->panelTheme;

    IntRect bounds = getClientBounds();

    const IntRect & uvRect = ct.statesUV[0];
    Vector2u ts = theme->getTextureAtlasSize();

    batch.fillNineSlices(bounds, ct.slicing, uvRect, ts);
}

//------------------------------------------------------------------------------
void Panel::onMousePress(Event & e)
{
    if (m_isResizeable)
    {
        Vector2i cursorPos(e.value.mouse.x, e.value.mouse.y);
        if (checkResizing(cursorPos))
        {
            beginCapture();
        }
    }
    e.consume();
}

//------------------------------------------------------------------------------
void Panel::onMouseRelease(Event & e)
{
    if (getControlFlag(TGUI_CF_CAPTURED))
    {
        m_resizeDirections.reset();
        endCapture();
    }
    e.consume();
}

//------------------------------------------------------------------------------
void Panel::onMouseMove(Event & e)
{
    if (isResizing())
    {
        IntRect b = getLocalClientBounds();
        Vector2i pos = Vector2i(e.value.mouse.x, e.value.mouse.y);// - getPosition();

        if (m_resizeDirections[TGUI_RIGHT])
            b.width() = pos.x() - b.x();

        if (m_resizeDirections[TGUI_BOTTOM])
            b.height() = pos.y() - b.y();

        if (m_resizeDirections[TGUI_LEFT])
        {
            b.width() -= pos.x() - b.x();
            b.x() = pos.x();
        }

        if (m_resizeDirections[TGUI_TOP])
        {
            b.height() -= pos.y() - b.y();
            b.y() = pos.y();
        }

        setLocalClientBounds(b);
        layoutChildren();

        e.consume();
    }
}

//------------------------------------------------------------------------------
bool Panel::checkResizing(Vector2i cursorPos)
{
    IntRect clientBounds = getClientBounds();
    if (clientBounds.contains(cursorPos))
    {
        if (cursorPos.x() >= clientBounds.maxX() - m_sideResizerSize)
            m_resizeDirections[TGUI_RIGHT] = true;

        if (cursorPos.y() >= clientBounds.maxY() - m_sideResizerSize)
            m_resizeDirections[TGUI_BOTTOM] = true;

        if (cursorPos.x() < clientBounds.minX() + m_sideResizerSize)
            m_resizeDirections[TGUI_LEFT] = true;

        if (cursorPos.y() < clientBounds.minY() + m_sideResizerSize)
            m_resizeDirections[TGUI_TOP] = true;

        return m_resizeDirections.any();
    }
    return false;
}

//------------------------------------------------------------------------------
void Panel::serializeState(JsonBox::Value & o, const sn::SerializationContext & ctx)
{
    Control::serializeState(o, ctx);
    sn::serialize(o["isResizeable"], m_isResizeable);
}

//------------------------------------------------------------------------------
void Panel::unserializeState(JsonBox::Value & o, const sn::SerializationContext & ctx)
{
    Control::unserializeState(o, ctx);
    sn::unserialize(o["isResizeable"], m_isResizeable);
}

} // namespace tgui


