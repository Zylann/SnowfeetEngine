#include <core/util/stringutils.h>
#include <core/util/typecheck.h>
#include <core/asset/AssetDatabase.h>
#include <core/sml/SmlParser.h>
#include "Theme.h"

using namespace sn;

namespace tgui
{

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void ControlTheme::serialize(sn::Variant & o) const
{
    tgui::serialize(o["slicing"], slicing);

    Variant & statesUVData = o["statesUV"];
    for (u32 i = 0; i < STATE_COUNT; ++i)
    {
        sn::serialize(statesUVData[i], statesUV[i]);
    }
}

void ControlTheme::unserialize(const sn::Variant & o)
{
    tgui::unserialize(o["slicing"], slicing);

    const Variant & statesUVData = o["statesUV"];
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

    Variant o;
	SmlParser parser;
	parser.parseValue(ifs, o);

    SerializationContext ctx(asset.getAssetMetadata().project);

    unserialize(o["textFormat"], theme->textFormat);
    sn::unserialize(o["textColor"], theme->textColor);

    theme->controlTheme.unserialize(o["controlTheme"]);
    theme->panelTheme.unserialize(o["panelTheme"]);

    const Variant & sliderData = o["sliderTheme"];
    if (sliderData.isDictionary())
    {
        theme->sliderBar.unserialize(sliderData["bar"]);
        theme->sliderThumbs.unserialize(sliderData["thumb"]);
        theme->sliderStep.unserialize(sliderData["step"]);
    }

    const Variant & textAreaData = o["textAreaTheme"];
    if (textAreaData.isDictionary())
    {
        theme->textAreaBackground.unserialize(textAreaData["background"]);
        theme->textAreaCaret.unserialize(textAreaData["caret"]);
    }

    // Get material
    sn::render::Material * mat = sn::getAssetBySerializedLocation<sn::render::Material>(o["material"].getString(), ctx.getProject());
    if (mat)
        theme->setMaterial(*mat);
    else
        SN_WERROR(L"No material specified in theme " << asset.getAssetMetadata().path);

    // Get font
    Font * font = sn::getAssetBySerializedLocation<Font>(o["font"].getString(), ctx.getProject(), true);
    if (font)
        theme->setFont(*font);
    else
        SN_WERROR(L"Font not found in theme " << asset.getAssetMetadata().path);

    // TODO This can be picked from the material now
    // Get texture size
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
