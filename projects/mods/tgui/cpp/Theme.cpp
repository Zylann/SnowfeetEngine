#include <core/util/stringutils.hpp>
#include <core/util/typecheck.hpp>
#include <core/asset/AssetDatabase.hpp>
#include "Theme.hpp"

using namespace sn;

namespace tgui
{

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void ControlTheme::serialize(JsonBox::Value & o) const
{
    tgui::serialize(o["slicing"], slicing);

    JsonBox::Value & statesUVData = o["statesUV"];
    for (u32 i = 0; i < STATE_COUNT; ++i)
    {
        sn::serialize(statesUVData[i], statesUV[i]);
    }
}

void ControlTheme::unserialize(JsonBox::Value & o)
{
    tgui::unserialize(o["slicing"], slicing);

    JsonBox::Value & statesUVData = o["statesUV"];
    if (statesUVData.isArray())
    {
        u32 n = statesUVData.getArray().size();
        if (n > STATE_COUNT)
            n = STATE_COUNT;
        for (u32 i = 0; i < n; ++i)
        {
            sn::unserialize(statesUVData[i], statesUV[i]);
        }
    }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

bool ThemeLoader::canLoad(const sn::AssetMetadata & meta) const
{
    String ext = getFileExtension(meta.path);
    return ext == L".tguitheme";
}

bool ThemeLoader::load(std::ifstream & ifs, sn::Asset & asset) const
{
    Theme * theme = checked_cast<Theme*>(&asset);
    SN_ASSERT(theme != nullptr, "Invalid state");

    JsonBox::Value o;
    o.loadFromStream(ifs);

    SerializationContext ctx(asset.getAssetMetadata().module);

    theme->controlTheme.unserialize(o["controlTheme"]);

    Material * mat = sn::getAssetBySerializedLocation<sn::Material>(o["material"].getString(), ctx.getModule(), this);
    if (mat)
        theme->setMaterial(*mat);
    else
        SN_WERROR(L"No material specified in theme " << asset.getAssetMetadata().path);

    Vector2u ts;
    sn::unserialize(o["textureSize"], ts);
    if (ts.x() == 0 || ts.y() == 0)
    {
        SN_WERROR(L"Invalid texture size specified in theme " << asset.getAssetMetadata().path);
        SN_ERROR("Got size " << sn::toString(ts));
    }
    else
    {
        theme->m_textureSize = ts;
    }

    return true;
}

} // namespace tgui
