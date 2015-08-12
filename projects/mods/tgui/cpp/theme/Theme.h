#ifndef __HEADER_TGUI_THEME__
#define __HEADER_TGUI_THEME__

#include <core/asset/base/Material.h>
#include <core/asset/base/Font.h>
#include <core/asset/SerializationContext.h>
#include <core/asset/AssetLoader.h>
#include <core/util/WeakRef.h>
#include <core/math/Rect.h>

#include "../Border.h"

namespace tgui
{

// TODO A rewrite of the theming system is planned. Use something closer to CSS and styles?

//-----------------------------------------------------------------------------
/// \brief Generic properties of a Control's theme
struct ControlTheme
{
    enum State
    {
        STATE_NORMAL = 0,
        STATE_HOVERED = 1,
        STATE_PRESSED = 2,
        STATE_DISABLED = 3,
        STATE_COUNT = 4
    };

    // TODO Include default sizes

    Border slicing;
    sn::IntRect statesUV[STATE_COUNT];

    void serialize(JsonBox::Value & o) const;
    void unserialize(const sn::Variant & o);

    //sn::IntRect getStateUV(u32 state);
};

//-----------------------------------------------------------------------------
/// \brief Theme description.
/// This kind of GUI uses only one atlassed texture for control backgrounds.
class Theme : public sn::Asset
{
public:
    SN_SCRIPT_OBJECT(tgui::Theme, sn::Asset)

    Theme():
        m_textureSize(256,256) // Arbitrary by default
    {}

    void setMaterial(sn::Material & m) { r_material.set(&m); }
    sn::Material * getMaterial() const { return r_material.isNull() ? nullptr : r_material.get(); }

    sn::Vector2u getTextureAtlasSize() const { return m_textureSize; }

    void setFont(sn::Font & font) { r_font.set(&font); }
    sn::Font * getFont() const { return r_font.isNull() ? nullptr : r_font.get(); }

public:
    sn::FontFormat textFormat;
    sn::Color textColor;

    ControlTheme controlTheme;
    ControlTheme panelTheme;
    ControlTheme sliderBar;
    ControlTheme sliderThumbs;
    ControlTheme textAreaBackground;
    ControlTheme textAreaCaret;

private:
    friend class ThemeLoader;

    sn::Vector2u m_textureSize;
    sn::WeakRef<sn::Material> r_material;
    sn::WeakRef<sn::Material> r_textMaterial;
    sn::WeakRef<sn::Font> r_font;

};

//-----------------------------------------------------------------------------
class ThemeLoader : public sn::AssetLoader
{
public:
    SN_OBJECT(tgui::ThemeLoader, sn::AssetLoader)

    const sn::ObjectType & getBaseAssetType() const override { return sn::getObjectType<Theme>(); }
    bool canLoad(const sn::AssetMetadata & meta) const override;
    bool load(std::ifstream & ifs, sn::Asset & asset) const override;
};

} // namespace tgui

#endif // __HEADER_TGUI_THEME__

