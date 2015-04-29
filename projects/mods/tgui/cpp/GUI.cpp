#include "GUI.hpp"
#include <core/system/gui/SystemGUI.hpp>
#include <core/asset/AssetDatabase.hpp>

using namespace sn;

namespace tgui
{

void GUI::onReady()
{

}

const Theme & GUI::getTheme() const
{
    if (m_theme.isNull())
        return m_defaultTheme;
    return *m_theme.get();
}

void GUI::draw(sn::IDrawContext & dc)
{
    const Theme & theme = getTheme();

    Material * themeMaterial = theme.getMaterial();
    if (themeMaterial)
    {
        Window * win = SystemGUI::get().getWindowByID(0);

        Vector2u screenSize = win->getClientSize();

        Matrix4 projection;
        projection.loadOrtho2DProjection(0, 0, screenSize.x(), screenSize.y(), -1, 100);
        dc.setProjectionMatrix(projection);

        dc.setMaterial(*themeMaterial);
        onDraw(dc);
    }
}

bool GUI::onSystemEvent(const sn::Event & systemEvent)
{
    tgui::Event ev;
    ev.value = systemEvent;

    dispatchEventToChildren(ev);

    return ev.consumed;
}

void GUI::serializeState(JsonBox::Value & o, const sn::SerializationContext & ctx)
{
    Control::serializeState(o, ctx);
    // TODO Serialize theme location
}

void GUI::unserializeState(JsonBox::Value & o, const sn::SerializationContext & ctx)
{
    Control::unserializeState(o, ctx);
    m_theme.set(getAssetBySerializedLocation<Theme>(o["theme"].getString(), ctx.getModule(), this));
}

} // namespace tgui


