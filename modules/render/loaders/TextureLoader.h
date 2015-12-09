#ifndef __HEADER_SNR_TEXTURELOADER__
#define __HEADER_SNR_TEXTURELOADER__

#include <core/asset/AssetLoader.h>

namespace sn
{

class TextureLoader : public AssetLoader
{
public:
    SN_OBJECT(sn::TextureLoader, sn::AssetLoader)

    const ObjectType & getBaseAssetType() const override;
    const ObjectType & getAssetInstanceType() const override;
    bool canLoad(const AssetMetadata & meta) const override;
    bool isDirect(const AssetMetadata & meta) const override;
    bool load(std::ifstream & ifs, Asset & asset) const override;
    //s32 getPriority(const AssetLoader & other) const override;
};

} // namespace sn

#endif // __HEADER_SNR_TEXTURELOADER__

