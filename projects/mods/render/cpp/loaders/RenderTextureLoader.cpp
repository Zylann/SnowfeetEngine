#include "RenderTextureLoader.h"
#include "../RenderTexture.h"
#include <core/util/stringutils.h>
#include <core/util/typecheck.h>

namespace sn {
namespace render {

const ObjectType & RenderTextureLoader::getAssetInstanceType() const
{
    return sn::getObjectType<RenderTexture>();
}

bool RenderTextureLoader::canLoad(const AssetMetadata & meta) const
{
    String ext = getFileExtension(meta.path);
    return ext == L".rendertexture";
}

bool RenderTextureLoader::load(std::ifstream & ifs, Asset & asset) const
{
    RenderTexture * rt = checked_cast<RenderTexture*>(&asset);
    SN_ASSERT(rt != nullptr, "Invalid asset or null given to RenderTextureLoader");

    JsonBox::Value doc;
    doc.loadFromStream(ifs);

	Vector2i size;
	sn::unserialize(doc["size"], size);
	if (size.x() <= 0 && size.y() <= 0)
		return false;

    bool hasDepth = false;
	sn::unserialize(doc["hasDepth"], hasDepth);

    rt->create(Vector2u(size.x(), size.y()), hasDepth);

    return true;
}

} // namespace render
} // namespace sn

