#include <core/system/gui/SystemGUI.hpp>
#include <core/asset/AssetDatabase.hpp>

#include "GUI.h"
#include "DrawBatch.h"

using namespace sn;

namespace tgui
{

//------------------------------------------------------------------------------
GUI::GUI()
{
    m_defaultTheme = new Theme();
}

//------------------------------------------------------------------------------
GUI::~GUI()
{
    delete m_defaultTheme;
}

//------------------------------------------------------------------------------
void GUI::onReady()
{
    // Setup event filter
    u32 events[] {
        SN_EVENT_MOUSE_DOWN,
        SN_EVENT_MOUSE_UP,
        SN_EVENT_MOUSE_MOVED,
        SN_EVENT_MOUSE_ENTERED,
        SN_EVENT_MOUSE_LEFT,
        SN_EVENT_MOUSE_WHEEL_MOVED
    };
    for (u32 i = 0; i < sizeof(events) / sizeof(u32); ++i)
    {
        m_eventFilter[events[i]] = true;
    }

    listenToSystemEvents();
}

//------------------------------------------------------------------------------
const Theme & GUI::getTheme() const
{
    if (m_theme.isNull())
    {
        SN_ASSERT(m_defaultTheme != nullptr, "Cannot get a theme now");
        return *m_defaultTheme;
    }
    return *m_theme.get();
}

//------------------------------------------------------------------------------
void GUI::draw(sn::IDrawContext & dc)
{
    const Theme & theme = getTheme();

    Material * themeMaterial = theme.getMaterial();
    if (themeMaterial)
    {
        Window * win = SystemGUI::get().getWindowByID(0);

        Vector2u screenSize = win->getClientSize();

        // Override projection
        Matrix4 projection;
        projection.loadOrtho2DProjection(0, 0, static_cast<f32>(screenSize.x()), static_cast<f32>(screenSize.y()), -1, 100);
        dc.setProjectionMatrix(projection);

        // Override view
        Matrix4 view;
        // TODO This translation quickfixes Matrix4 not handling assymetric ortho frustrum, please fix it
        view.setTranslation(sn::Vector3f(-static_cast<f32>(screenSize.x())/2, -static_cast<f32>(screenSize.y())/2, 0));
        dc.setViewMatrix(view);

        DrawBatch batch(dc);
        batch.setMaterial(*themeMaterial);

        onDraw(batch);
        batch.flush();
    }
}

//------------------------------------------------------------------------------
bool GUI::onSystemEvent(const sn::Event & systemEvent)
{
    if (m_eventFilter[systemEvent.type])
    {
        tgui::Event ev;
        ev.value = systemEvent;

        dispatchEventToChildren(ev);

        return ev.consumed;
    }
    return false;
}

//------------------------------------------------------------------------------
void GUI::serializeState(JsonBox::Value & o, const sn::SerializationContext & ctx)
{
    Control::serializeState(o, ctx);
    // TODO Serialize theme location
}

//------------------------------------------------------------------------------
void GUI::unserializeState(JsonBox::Value & o, const sn::SerializationContext & ctx)
{
    Control::unserializeState(o, ctx);
    m_theme.set(getAssetBySerializedLocation<Theme>(o["theme"].getString(), ctx.getModule(), this));
}

} // namespace tgui


