#ifndef __HEADER_FREETYPE_FONTLOADER__
#define __HEADER_FREETYPE_FONTLOADER__

#include <core/asset/AssetLoader.h>

namespace sn
{

class FontLoader : public sn::AssetLoader
{
public:
    SN_OBJECT

    FontLoader();
    ~FontLoader();

    const sn::ObjectType & getBaseAssetType() const override;
    const sn::ObjectType & getAssetInstanceType() const override;
    bool canLoad(const sn::AssetMetadata & meta) const override;
    bool load(std::ifstream & ifs, sn::Asset & asset) const override;

private:
    void * m_library;

};

} // sn

#endif // __HEADER_FREETYPE_FONTLOADER__

