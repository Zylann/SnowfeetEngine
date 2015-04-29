#include "DrawBatch.hpp"

using namespace sn;

namespace tgui
{

DrawBatch::DrawBatch(IDrawContext & dc):
    m_dc(dc)
{
    m_mesh = new sn::Mesh();
    m_mesh->setPrimitiveType(SN_MESH_QUADS);
}

DrawBatch::~DrawBatch()
{
    delete m_mesh;
}

void DrawBatch::setMaterial(sn::Material & m)
{
    m_dc.setMaterial(m);
}

void DrawBatch::fillRect(const IntRect & r, const IntRect & texRect, Vector2u ts)
{
    Mesh & m = *m_mesh;

    m.addPosition(static_cast<f32>(r.minX()), static_cast<f32>(r.minY()), 0.f);
    m.addPosition(static_cast<f32>(r.maxX()), static_cast<f32>(r.minY()), 0.f);
    m.addPosition(static_cast<f32>(r.maxX()), static_cast<f32>(r.maxY()), 0.f);
    m.addPosition(static_cast<f32>(r.minX()), static_cast<f32>(r.maxY()), 0.f);

    FloatRect uvRectf = FloatRect::fromMinMax(
        static_cast<f32>(texRect.minX()) / static_cast<f32>(ts.x()),
        static_cast<f32>(texRect.minY()) / static_cast<f32>(ts.y()),
        static_cast<f32>(texRect.maxX()) / static_cast<f32>(ts.x()),
        static_cast<f32>(texRect.maxY()) / static_cast<f32>(ts.y())
    );

    m.addTexCoord(uvRectf.minX(), uvRectf.minY());
    m.addTexCoord(uvRectf.maxX(), uvRectf.minY());
    m.addTexCoord(uvRectf.maxX(), uvRectf.maxY());
    m.addTexCoord(uvRectf.minX(), uvRectf.maxY());

    m.addColor(Color(1, 1, 1));
    m.addColor(Color(1, 1, 1));
    m.addColor(Color(1, 1, 1));
    m.addColor(Color(1, 1, 1));
}

void DrawBatch::flush()
{
    sn::Mesh & m = *m_mesh;
    m.recalculateIndexes();
    m_dc.drawMesh(m);
    m.clear();
}

} // namespace tgui

