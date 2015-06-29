#ifndef __HEADER_SNR_TEXTURELOADER__
#define __HEADER_SNR_TEXTURELOADER__

#include <core/asset/AssetLoader.h>

namespace sn {
namespace render {

class TextureLoader : public AssetLoader
{
public:
    SN_OBJECT(sn::render::TextureLoader, sn::AssetLoader)

    const ObjectType & getBaseAssetType() const override;
    const ObjectType & getAssetInstanceType() const override;
    bool canLoad(const AssetMetadata & meta) const override;
    bool isDirect(const AssetMetadata & meta) const override;
    bool load(std::ifstream & ifs, Asset & asset) const override;
    //s32 getPriority(const AssetLoader & other) const override;
};

} // namespace render
} // namespace sn

#endif // __HEADER_SNR_TEXTURELOADER__

