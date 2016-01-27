#ifndef __HEADER_TGUI_DRAWBATCH__
#define __HEADER_TGUI_DRAWBATCH__

#include <core/math/Color.h>

#include <modules/render/Mesh.h>
#include <modules/render/Material.h>
#include <modules/render/VideoDriver.h>
#include <modules/freetype/Font.hpp>

#include <modules/tgui/Border.h>
#include <modules/tgui/TextAlignment.h>
#include <modules/tgui/TextWrapper.h>

namespace tgui
{

class SN_TGUI_API DrawBatch
{
public:
    DrawBatch(sn::VideoDriver & driver);
    ~DrawBatch();

    void setViewMatrix(const sn::Matrix4 & matrix);
    void setProjectionMatrix(const sn::Matrix4 & matrix);

    void setMaterial(sn::Material & m);
    void flush();
    
    void fillRect(
        const sn::IntRect & r, 
        const sn::IntRect & texRect, 
        sn::Vector2u ts, 
        sn::Color color = sn::Color(1,1,1,1)
    );
    
    void fillNineSlices(
        const sn::IntRect & r, 
        const Border & b, 
        const sn::IntRect & texRect, 
        sn::Vector2u ts
    );
    
    void drawTextLine(
        const char * str,
        sn::u32 charCount,
        sn::IntRect area, 
        const sn::Font & font, 
        sn::FontFormat format, 
        TextAlignment align = TGUI_ALIGN_LEFT,
        sn::Color color = sn::Color(1,1,1,1),
        bool swapFontTexture = true // Internal
    );

    // Helper (uses drawTextLine())
    void drawText(
        const TextModel & model,
        sn::IntRect area,
        const sn::Font & font,
        sn::FontFormat format,
        TextAlignment align = TGUI_ALIGN_LEFT,
        sn::Color color = sn::Color(1,1,1,1)
    );

    void drawText(
        const TextWrapper & wrapper,
        sn::IntRect area,
        const sn::Font & font,
        sn::FontFormat format,
        TextAlignment align = TGUI_ALIGN_LEFT,
        sn::Color color = sn::Color(1,1,1,1)
    );

    void setScissor(sn::IntRect rect, sn::u32 windowID);
    void disableScissor();

private:
    void setTexture(sn::Texture * tex);
    sn::Texture * getTexture() const;

private:
    sn::Mesh * m_mesh;
    sn::VideoDriver & r_driver;
    sn::Material * r_material;
    sn::Matrix4 m_viewMatrix;
    sn::Matrix4 m_projectionMatrix;

};

} // namespace tgui

#endif // __HEADER_TGUI_DRAWBATCH__

