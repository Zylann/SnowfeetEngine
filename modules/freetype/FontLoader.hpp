#ifndef __HEADER_FREETYPE_FONTLOADER__
#define __HEADER_FREETYPE_FONTLOADER__

#include <core/asset/AssetLoader.h>

namespace freetype
{

class FontLoader : public sn::AssetLoader
{
public:
    SN_OBJECT(freetype::FontLoader, sn::AssetLoader)

    FontLoader();
    ~FontLoader();

    const sn::ObjectType & getBaseAssetType() const override;
    const sn::ObjectType & getAssetInstanceType() const override;
    bool canLoad(const sn::AssetMetadata & meta) const override;
    bool load(std::ifstream & ifs, sn::Asset & asset) const override;

private:
    void * m_library;

};

} // freetype

#endif // __HEADER_FREETYPE_FONTLOADER__

