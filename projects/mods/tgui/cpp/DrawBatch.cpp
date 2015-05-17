#include "DrawBatch.h"
#include <core/system/gui/SystemGUI.hpp>
#include <core/util/stringutils.hpp>

using namespace sn;

namespace tgui
{

//------------------------------------------------------------------------------
DrawBatch::DrawBatch(IDrawContext & dc):
    m_dc(dc),
    r_material(nullptr)
{
    m_mesh = new sn::Mesh();
    m_mesh->setPrimitiveType(SN_MESH_QUADS);
}

//------------------------------------------------------------------------------
DrawBatch::~DrawBatch()
{
    delete m_mesh;
}

//------------------------------------------------------------------------------
void DrawBatch::setMaterial(sn::Material & m)
{
    m_dc.setMaterial(m);
    r_material = &m;
}

//------------------------------------------------------------------------------
void DrawBatch::fillRect(const IntRect & r, const IntRect & texRect, Vector2u ts, Color color)
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

    m.addColor(color);
    m.addColor(color);
    m.addColor(color);
    m.addColor(color);
}

//------------------------------------------------------------------------------
void DrawBatch::fillNineSlices(const sn::IntRect & r, const Border & b, const sn::IntRect & texRect, sn::Vector2u ts)
{
    // TODO Optimize, this can be written with plain arrays and added to the mesh

    if (b.left && b.top)
    {
        fillRect(
            IntRect::fromPositionSize(r.minX(), r.minY(), b.left, b.top),
            IntRect::fromPositionSize(texRect.x(), texRect.y(), b.left, b.top),
            ts
        );
    }

    if (b.right && b.top)
    {
        fillRect(
            IntRect::fromPositionSize(r.maxX() - b.right, r.minY(), b.right, b.top),
            IntRect::fromPositionSize(texRect.maxX() - b.right, texRect.y(), b.right, b.top),
            ts
        );
    }

    if (b.left && b.bottom)
    {
        fillRect(
            IntRect::fromPositionSize(r.minX(), r.maxY() - b.bottom, b.left, b.bottom),
            IntRect::fromPositionSize(texRect.x(), texRect.maxY() - b.bottom, b.left, b.bottom),
            ts
        );
    }

    if (b.right && b.bottom)
    {
        fillRect(
            IntRect::fromPositionSize(r.maxX() - b.right, r.maxY() - b.bottom, b.right, b.bottom),
            IntRect::fromPositionSize(texRect.maxX() - b.right, texRect.maxY() - b.bottom, b.right, b.bottom),
            ts
        );
    }

    if (b.left)
    {
        fillRect(
            IntRect::fromPositionSize(r.minX(), r.minY() + b.top, b.left, r.height() - b.top - b.bottom),
            IntRect::fromPositionSize(texRect.minX(), texRect.minY() + b.top, b.left, texRect.height() - b.top - b.bottom),
            ts
        );
    }

    if (b.right)
    {
        fillRect(
            IntRect::fromPositionSize(r.maxX() - b.right, r.minY() + b.top, b.right, r.height() - b.top - b.bottom),
            IntRect::fromPositionSize(texRect.maxX() - b.right, texRect.minY() + b.top, b.right, texRect.height() - b.top - b.bottom),
            ts
        );
    }

    if (b.top)
    {
        fillRect(
            IntRect::fromPositionSize(r.minX() + b.left, r.minY(), r.width() - b.left - b.right, b.top),
            IntRect::fromPositionSize(texRect.minX() + b.left, texRect.minY(), texRect.width() - b.left - b.right, b.top),
            ts
        );
    }

    if (b.bottom)
    {
        fillRect(
            IntRect::fromPositionSize(r.minX() + b.left, r.maxY() - b.bottom, r.width() - b.left - b.right, b.bottom),
            IntRect::fromPositionSize(texRect.minX() + b.left, texRect.maxY() - b.bottom, texRect.width() - b.left - b.right, b.bottom),
            ts
        );
    }

    fillRect(
        IntRect::fromPositionSize(r.minX() + b.left, r.minY() + b.top, r.width() - b.left - b.right, r.height() - b.top - b.bottom),
        IntRect::fromPositionSize(texRect.minX() + b.left, texRect.minY() + b.top, texRect.width() - b.left - b.right, texRect.height() - b.top - b.bottom),
        ts
    );

}

//------------------------------------------------------------------------------
/////////////////////////////////
void DrawBatch::drawTextLine(
    const char *   str, 
    u32            charCount, 
    IntRect        area, 
    const Font &   font, 
    FontFormat     format, 
    TextAlignment  align, 
    Color          color,
    bool           swapFontTexture
)
/////////////////////////////////
{
    if (r_material == nullptr)
        return;
    if (charCount == 0)
        return;
    SN_ASSERT(str != nullptr, "Reveived null string");

    TextureBase * tex = font.getTexture(format);
    if (tex == nullptr)
        return;

    // TODO Improve DrawBatch so we don't have to swap textures like this

    TextureBase * lastTexture = nullptr;
    if (swapFontTexture)
    {
        lastTexture = getTexture();
        setTexture(tex);
    }
    
    Vector2u ts = tex->getSize();

    Vector2i pos = area.origin();
    pos.y() += font.getLineHeight(format.size);

    if (align != TGUI_ALIGN_LEFT)
    {
        s32 w = font.getLineWidth(str, charCount, format);
        if (align == TGUI_ALIGN_RIGHT)
            pos.x() = area.maxX() - w;
        else
            pos.x() = area.x() + (area.width() - w) / 2;
    }

    for (u32 i = 0; i < charCount; ++i)
    {
        char c = str[i];
        if (!isEOL(c))
        {
            const Glyph & glyph = font.getGlyph(c, format);

            IntRect rect = glyph.bounds;
            rect.origin() += pos;

            fillRect(rect, glyph.imageRect, ts, color);

            pos.x() += glyph.advance;
        }
    }

    if (swapFontTexture)
        setTexture(lastTexture);
}

//------------------------------------------------------------------------------
/////////////////////////////////
void DrawBatch::drawText(
    const TextModel &  model,
    IntRect            area,
    const Font &       font,
    FontFormat         format,
    TextAlignment      align,
    Color              color
)
/////////////////////////////////
{
    TextureBase * tex = font.getTexture(format);
    if (tex == nullptr)
        return;

    s32 lineHeight = font.getLineHeight(format.size);

    TextureBase * lastTexture = getTexture();
    setTexture(tex);

    for (u32 i = 0; i < model.getLineCount(); ++i)
    {
        const std::string & line = model.getLine(i);

        drawTextLine(
            line.c_str(),
            line.size(),
            area,
            font,
            format,
            align,
            color,
            false // Don't set the font's texture (batching)
        );

        if (lineHeight > area.height())
            break;
        area.y() += lineHeight;
        area.height() -= lineHeight;
    }

    setTexture(lastTexture);
}

//------------------------------------------------------------------------------
void DrawBatch::setTexture(TextureBase * tex)
{
    SN_ASSERT(r_material != nullptr, "Cannot set texture when material is not set");
    
    const TextureBase * lastTexture = r_material->getTexture(Material::MAIN_TEXTURE);
    if (tex != lastTexture)
    {
        flush();
        r_material->setTexture(Material::MAIN_TEXTURE, tex);
        // TODO Needed to update uniforms...
        m_dc.setMaterial(*r_material);
    }
}

//------------------------------------------------------------------------------
sn::TextureBase * DrawBatch::getTexture() const
{
    SN_ASSERT(r_material != nullptr, "Cannot get texture when material is not set");
    return r_material->getTexture(Material::MAIN_TEXTURE);
}

//------------------------------------------------------------------------------
void DrawBatch::setScissor(sn::IntRect rect, u32 windowID)
{
    // The batch must be flushed before because previous geometry is not necessarily affected by scissor
    flush();

    if (rect.width() < 0)
        rect.width() = 0;
    if (rect.height() < 0)
        rect.height() = 0;

    sn::Window * win = SystemGUI::get().getWindowByID(windowID);
    if (win)
    {
        Vector2u winSize = win->getClientSize();
        rect.y() = static_cast<s32>(winSize.y()) - rect.y() - rect.height();
        m_dc.setScissor(rect);
    }
}

//------------------------------------------------------------------------------
void DrawBatch::disableScissor()
{
    flush();
    m_dc.disableScissor();
}

//------------------------------------------------------------------------------
void DrawBatch::flush()
{
    sn::Mesh & m = *m_mesh;
    m.recalculateIndexes();
    m_dc.drawMesh(m);
    m.clear();
}

} // namespace tgui

