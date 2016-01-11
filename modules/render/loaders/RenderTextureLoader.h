#ifndef __HEADER_SNR_RENDERTEXTURELOADER__
#define __HEADER_SNR_RENDERTEXTURELOADER__

#include <core/asset/AssetLoader.h>

namespace sn
{

class RenderTexture;

class RenderTextureLoader : public AssetLoader
{
public:
    SN_OBJECT

    const ObjectType & getAssetInstanceType() const override;
    bool canLoad(const AssetMetadata & meta) const override;
    bool load(std::ifstream & ifs, Asset & asset) const override;

private:
    bool loadFromVariant(sn::Variant & doc, RenderTexture & rt) const;

};

} // namespace sn

#endif // __HEADER_SNR_RENDERTEXTURELOADER__

