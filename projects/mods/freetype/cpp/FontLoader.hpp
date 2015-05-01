#ifndef __HEADER_FREETYPE_FONTLOADER__
#define __HEADER_FREETYPE_FONTLOADER__

#include <core/asset/AssetLoader.hpp>

namespace freetype
{

class FontLoader : public sn::AssetLoader
{
public:
    SN_OBJECT(freetype::FontLoader, sn::AssetLoader)

    const sn::ObjectType & getBaseAssetType() const override;
    bool canLoad(const sn::AssetMetadata & meta) const override;
    bool load(std::ifstream & ifs, sn::Asset & asset) const override;

};

} // freetype

#endif // __HEADER_FREETYPE_FONTLOADER__

