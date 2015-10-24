#include <core/util/stringutils.h>
#include <core/util/typecheck.h>
#include <core/sml/SmlParser.h>
#include <core/sml/variant_serialize.h>

#include "RenderTextureLoader.h"
#include "../RenderTexture.h"

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

    SmlParser parser;
    sn::Variant doc;
    parser.parseValue(ifs, doc);

    return loadFromVariant(doc, *rt);
}

bool RenderTextureLoader::loadFromVariant(sn::Variant & doc, RenderTexture & rt) const
{
	Vector2i size;
	sn::unserialize(doc["size"], size);
	if (size.x() <= 0 && size.y() <= 0)
		return false;

    bool hasDepth = false;
	sn::unserialize(doc["hasDepth"], hasDepth);

    rt.create(Vector2u(size.x(), size.y()), hasDepth);

    return true;
}

} // namespace render
} // namespace sn

