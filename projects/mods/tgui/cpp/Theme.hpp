#ifndef __HEADER_TGUI_THEME__
#define __HEADER_TGUI_THEME__

#include <core/asset/base/Material.hpp>
#include <core/asset/SerializationContext.hpp>
#include <core/asset/AssetLoader.hpp>
#include <core/util/WeakRef.hpp>
#include <core/math/Rect.hpp>

#include "Border.hpp"

namespace tgui
{

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

    Border slicing;
    sn::IntRect statesUV[STATE_COUNT];

    void serialize(JsonBox::Value & o) const;
    void unserialize(JsonBox::Value & o);
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

    ControlTheme controlTheme;

private:
    friend class ThemeLoader;

    sn::Vector2u m_textureSize;
    sn::WeakRef<sn::Material> r_material;

};

//-----------------------------------------------------------------------------
class ThemeLoader : public sn::AssetLoader
{
public:
    SN_OBJECT(tgui::ThemeLoader, sn::AssetLoader)

    const sn::ObjectType & getBaseAssetType() const { return Theme::__sGetObjectType(); }
    bool canLoad(const sn::AssetMetadata & meta) const;
    bool load(std::ifstream & ifs, sn::Asset & asset) const;
};

} // namespace tgui

#endif // __HEADER_TGUI_THEME__

