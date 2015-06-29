#ifndef __HEADER_SNR_RENDERTEXTURELOADER__
#define __HEADER_SNR_RENDERTEXTURELOADER__

#include <core/asset/AssetLoader.h>

namespace sn {
namespace render {

class RenderTextureLoader : public AssetLoader
{
public:
    SN_OBJECT(sn::render::RenderTextureLoader, sn::AssetLoader)

    const ObjectType & getAssetInstanceType() const override;
    bool canLoad(const AssetMetadata & meta) const override;
    bool load(std::ifstream & ifs, Asset & asset) const override;
};

} // namespace render
} // namespace sn

#endif // __HEADER_SNR_RENDERTEXTURELOADER__

