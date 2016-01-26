#include <core/util/Profiler.h>
#include <core/system/SystemGUI.h>

#include "Panel.h"
#include "../GUI.h"

using namespace sn;

namespace tgui
{

SN_OBJECT_IMPL(Panel)

//------------------------------------------------------------------------------
Panel::Panel() : Control(),
    m_isResizeable(false),
    m_sideResizerSize(4), // Default
    m_cornerResizerSize(8) // Default
{
}

//------------------------------------------------------------------------------
void Panel::onDrawSelf(DrawBatch & batch)
{
    const Theme * theme = getTheme();
    if (theme == nullptr)
        return;
    
    SN_BEGIN_PROFILE_SAMPLE_NAMED("TGUI draw Panel");

    const ControlTheme & ct = theme->panelTheme;

    IntRect bounds = getClientBounds();

    const IntRect & uvRect = ct.statesUV[0];
    Vector2u ts = theme->getTextureAtlasSize();

    batch.fillNineSlices(bounds, ct.slicing, uvRect, ts);

    SN_END_PROFILE_SAMPLE();
}

//------------------------------------------------------------------------------
void Panel::onMousePress(Event & e)
{
    Vector2i cursorPos(e.value.mouse.x, e.value.mouse.y);

    if ((m_isResizeable && checkResizing(cursorPos, m_resizeDirections)) || m_isDraggable)
    {
        beginCapture(e);
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
    if (m_isResizeable)
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

        //e.consume();
    }

    if (!e.consumed && m_isDraggable && isPressed())
    {
        GUI * gui = getGUI();
        if (gui && gui->isDragThresholdReached())
        {
            Vector2i captureOrigin = gui->getCaptureOrigin();
            Vector2i initialOrigin = gui->getCaptureControlOrigin();
            Vector2i mousePos = Vector2i(e.value.mouse.x, e.value.mouse.y);

            Vector2i relativeCaptureOrigin = captureOrigin;
            Control * parent = getParentControl();
            if (parent)
                relativeCaptureOrigin -= parent->getPosition();

            //SN_LOG("Delta " << e.value.mouse.x << "," << e.value.mouse.y << " - " << e.value.mouse.lastX << "," << e.value.mouse.lastY);
            auto rect = getLocalClientBounds();
            rect.origin() = initialOrigin + mousePos - relativeCaptureOrigin;
            setLocalClientBounds(rect);

            e.consume();
        }
    }
}

//------------------------------------------------------------------------------
void Panel::onSetCursor(Event & e)
{
    if (!isResizing() && m_isResizeable)
    {
        std::bitset<TGUI_DIRECTION_COUNT> dirs;
        Vector2i pos = Vector2i(e.value.mouse.x, e.value.mouse.y);
        sn::Window * win = SystemGUI::get().getWindowByID(getWindowID());
        if (win)
        {
            CursorType cursor = SN_CURSOR_DEFAULT;

            if (checkResizing(pos, dirs))
            {
                if ((dirs[TGUI_LEFT] && dirs[TGUI_TOP]) || (dirs[TGUI_RIGHT] && dirs[TGUI_BOTTOM]))
                {
                    // Diagonal resize
                    cursor = SN_CURSOR_RESIZE_TOP_LEFT_BOTTOM_RIGHT;
                }
                else if ((dirs[TGUI_LEFT] && dirs[TGUI_BOTTOM]) || (dirs[TGUI_RIGHT] && dirs[TGUI_TOP]))
                {
                    // Diagonal resize
                    cursor = SN_CURSOR_RESIZE_BOTTOM_LEFT_TOP_RIGHT;
                }
                else if (dirs[TGUI_LEFT] || dirs[TGUI_RIGHT])
                {
                    // Horizontal resize
                    cursor = SN_CURSOR_RESIZE_HORIZONTAL;
                }
                else if (dirs[TGUI_TOP] || dirs[TGUI_BOTTOM])
                {
                    // Vertical resize
                    cursor = SN_CURSOR_RESIZE_VERTICAL;
                }

                e.consume();
            }

            win->setMouseCursor(cursor);
        }
    }
}

//------------------------------------------------------------------------------
bool Panel::checkResizing(Vector2i cursorPos, std::bitset<TGUI_DIRECTION_COUNT> & out_dirs)
{
    IntRect clientBounds = getClientBounds();
    if (clientBounds.contains(cursorPos))
    {
        if (cursorPos.x() >= clientBounds.maxX() - m_sideResizerSize)
            out_dirs[TGUI_RIGHT] = true;

        if (cursorPos.y() >= clientBounds.maxY() - m_sideResizerSize)
            out_dirs[TGUI_BOTTOM] = true;

        if (cursorPos.x() < clientBounds.minX() + m_sideResizerSize)
            out_dirs[TGUI_LEFT] = true;

        if (cursorPos.y() < clientBounds.minY() + m_sideResizerSize)
            out_dirs[TGUI_TOP] = true;

        return out_dirs.any();
    }
    return false;
}

//------------------------------------------------------------------------------
void Panel::serializeState(sn::Variant & o, const sn::SerializationContext & ctx)
{
    Control::serializeState(o, ctx);
    sn::serialize(o["isResizeable"], m_isResizeable);
    sn::serialize(o["isDraggable"], m_isDraggable);
}

//------------------------------------------------------------------------------
void Panel::unserializeState(const sn::Variant & o, const sn::SerializationContext & ctx)
{
    Control::unserializeState(o, ctx);
    sn::unserialize(o["isResizeable"], m_isResizeable);
    sn::unserialize(o["isDraggable"], m_isDraggable);
}

} // namespace tgui


