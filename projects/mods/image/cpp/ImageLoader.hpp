#ifndef __HEADER_SN_IMAGELOADER__
#define __HEADER_SN_IMAGELOADER__

#include <string>
#include <vector>

#include <core/types.hpp>
#include <core/math/Vector2.hpp>
#include <core/math/Color.hpp>
#include <core/asset/AssetLoader.hpp>
#include <core/asset/base/Image.hpp>

namespace sn
{

class ImageLoader : public AssetLoader
{
public:
    SN_OBJECT(sn::ImageLoader, sn::AssetLoader)

    const ObjectType & getBaseAssetType() const override;
    bool canLoad(const AssetMetadata & meta) const override;
    bool isDirect(const AssetMetadata & meta) const override;
    bool load(std::ifstream & ifs, Asset & asset) const override;

private:
    static bool loadFromMemory(sn::Image & image, const u8 * data, u32 dataSize);

};

} // namespace sn

#endif // __HEADER_SN_IMAGELOADER__

