#include "Button.hpp"
#include "GUI.hpp"
#include "DrawBatch.hpp"

using namespace sn;

namespace tgui
{

void Button::onMouseEnter(Event & e)
{

}

void Button::onMouseLeave(Event & e)
{

}

void Button::onMousePress(Event & e)
{
    SN_DLOG("Button pressed!");
}

void Button::onMouseRelease(Event & e)
{

}

void Button::onDrawSelf(sn::IDrawContext & dc)
{
    GUI * gui = getGUI();
    if (gui == nullptr)
        return;
    const Theme & theme = gui->getTheme();
    const ControlTheme & ct = theme.controlTheme;

    IntRect bounds = getClientBounds();

    //Matrix4 mat;
    //mat.setTranslation(Vector3f(bounds.minY(), bounds.minY(), 0));
    //dc.setModelMatrix(mat);

    DrawBatch batch(dc);

    u32 state = ControlTheme::STATE_NORMAL;
    if (isPressed())
        state = ControlTheme::STATE_PRESSED;
    else if (isHovered())
        state = ControlTheme::STATE_HOVERED;

    const IntRect & uvRect = ct.statesUV[state];
    Vector2u ts = theme.getTextureAtlasSize();

    batch.fillNineSlices(bounds, ct.slicing, uvRect, ts);
    batch.flush();

    /*
    Mesh * mesh = new Mesh();
    Mesh & m = *mesh;

    Vector3f positions[]{
        Vector3f(bounds.minX(), bounds.minY(), 0),
        Vector3f(bounds.maxX(), bounds.minY(), 0),
        Vector3f(bounds.maxX(), bounds.maxY(), 0),
        Vector3f(bounds.minX(), bounds.maxY(), 0)
    };

    Color colors[]{
        { 1, 1, 1, 1 },
        { 1, 1, 1, 1 },
        { 1, 1, 1, 1 },
        { 1, 1, 1, 1 },
    };

    const IntRect & uvRect = ct.statesUV[0];
    Vector2u ts = theme.getTextureAtlasSize();
    FloatRect uvRectf = FloatRect::fromMinMax(
        static_cast<f32>(uvRect.minX()) / static_cast<f32>(ts.x()),
        static_cast<f32>(uvRect.minY()) / static_cast<f32>(ts.y()),
        static_cast<f32>(uvRect.maxX()) / static_cast<f32>(ts.x()),
        static_cast<f32>(uvRect.maxY()) / static_cast<f32>(ts.y())
    );
    Vector2f uv[]{
        Vector2f(uvRectf.minX(), uvRectf.minY()),
        Vector2f(uvRectf.maxX(), uvRectf.minY()),
        Vector2f(uvRectf.maxX(), uvRectf.maxY()),
        Vector2f(uvRectf.minX(), uvRectf.maxY())
    };

    u32 indices[]{
        0, 1, 2,
        0, 2, 3
    };

    m.setPositions(positions, 4);
    m.setColors(colors, 4);
    m.setUV(uv, 4);
    m.setTriangleIndices(indices, 6);

    dc.drawMesh(m);

    mesh->release();
    */
}

void Button::serializeState(JsonBox::Value & o, const SerializationContext & ctx)
{
    Control::serializeState(o, ctx);

}

void Button::unserializeState(JsonBox::Value & o, const SerializationContext & ctx)
{
    Control::unserializeState(o, ctx);

}

} // namespace tgui



